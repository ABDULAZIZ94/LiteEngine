#include "PointVolumetricLightContainerRenderSubPass.h"

#include "Renderer/Pipeline.h"
#include "RenderPasses/GBuffer.h"

PointVolumetricLightContainerRenderSubPass::~PointVolumetricLightContainerRenderSubPass ()
{

}

RenderVolumeCollection* PointVolumetricLightContainerRenderSubPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	StartPointLightPass (rvc);

	/*
	 * Draw volumetric lights
	*/

	PointLightPass (renderScene, camera, rvc);

	/*
	 * End directional light pass
	*/

	EndPointLightPass ();

	return rvc;
}

bool PointVolumetricLightContainerRenderSubPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute directional volumetric light render sub pass
	*/

	return true;
}

void PointVolumetricLightContainerRenderSubPass::StartPointLightPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto lightAccumulationVolume = rvc->GetRenderVolume ("LightAccumulationVolume");

	lightAccumulationVolume->BindForWriting ();
}

void PointVolumetricLightContainerRenderSubPass::PointLightPass (const RenderScene* renderScene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Lock shader for volumetric point light
	*/

	LockShader (renderLightObject);

	/*
	 * Draw point light for stencil pass
	*/

	PointLightStencilPass (renderScene, camera, renderLightObject, rvc);

	/*
	 * Draw volumetric point light
	*/

	PointLightDrawPass (renderScene, camera, renderLightObject, rvc);
}

void PointVolumetricLightContainerRenderSubPass::EndPointLightPass ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();
}

void PointVolumetricLightContainerRenderSubPass::PointLightStencilPass (const RenderScene* renderScene, const Camera* camera,
	RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	/*
	 * No rendering target
	*/

	GL::DrawBuffer (GL_NONE);

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);

	/*
	 * Stencil pass technique. For more details please look at the link bellow.
	 * http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html
	 *
	 * It uses stencil test to identify the fragments that are in light.
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_ALWAYS, 0, 0);

	GL::StencilOpSeparate (GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	GL::StencilOpSeparate (GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	/*
	 * Both faces need to be drawn.
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Send camera to pipeline
	*/

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);

	/*
	 * Volumetric light draw.
	*/

	renderLightObject->Draw ();
}

void PointVolumetricLightContainerRenderSubPass::PointLightDrawPass (const RenderScene* renderScene, const Camera* camera,
	RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	/*
	 * Bind all render volumes
	*/

	for (RenderVolumeI* renderVolume : *rvc) {
		renderVolume->BindForReading ();
	}

	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto lightAccumulationVolume = rvc->GetRenderVolume ("LightAccumulationVolume");

	lightAccumulationVolume->BindForWriting ();

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Disable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);

	/*
	 * Pass the fragment only if it is like that
	 *
	 * 				(buffer frag) > (value '1')
	 *
	 * Don't override the sky, which have value '0'.
	 * Override only what fragments that are in light.
	 *
	 * After drawing, Stencil Buffer is refilled with 1's.
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_LESS, 1, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	/*
	 * Blend between point lights with same weight.
	*/

	GL::Enable (GL_BLEND);
	GL::BlendEquation (GL_FUNC_ADD);
	GL::BlendFunc (GL_ONE, GL_ONE);

	/*
	 * Process fragments single time. Front Face randomly chosen.
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_FRONT);

	/*
	 * Send camera to pipeline
	*/

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);

	/*
	 * Draw the volumetric light.
	*/

	renderLightObject->Draw ();
}
