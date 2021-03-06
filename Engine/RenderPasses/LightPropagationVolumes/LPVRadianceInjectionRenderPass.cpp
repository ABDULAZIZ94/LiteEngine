#include "LPVRadianceInjectionRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Core/Console/Console.h"

LPVRadianceInjectionRenderPass::LPVRadianceInjectionRenderPass () :
	_lpvVolume (new LPVVolume ()),
	_lpvGeometryVolume (new LPVGeometryVolume ())
{

}

LPVRadianceInjectionRenderPass::~LPVRadianceInjectionRenderPass ()
{
	delete _lpvGeometryVolume;
	delete _lpvVolume;
}

void LPVRadianceInjectionRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesRadianceInjectionFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);

	/*
	 * Initialize light propagation volume
	*/

	InitLPVVolume (settings);
}

void LPVRadianceInjectionRenderPass::Clear ()
{
	/*
	 * Clear post processing volume
	*/

	_lpvGeometryVolume->Clear ();
	_lpvVolume->Clear ();
}

RenderVolumeCollection* LPVRadianceInjectionRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 *
	*/

	UpdateLPVVolume (settings);

	/*
	 * Start screen space ambient occlusion generation pass
	*/

	StartPostProcessPass ();

	/*
	 * Screen space ambient occlusion generation pass
	*/

	PostProcessPass (renderScene, camera, settings, renderLightObject, rvc);

	/*
	 * End screen space ambient occlusion generation pass
	*/

	EndPostProcessPass ();

	return rvc->Insert ("LightPropagationVolume", _lpvVolume)
		->Insert ("LPVGeometryVolume", _lpvGeometryVolume);
}

bool LPVRadianceInjectionRenderPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void LPVRadianceInjectionRenderPass::StartPostProcessPass ()
{
	_lpvGeometryVolume->ClearVolume ();
	_lpvVolume->ClearVolume ();

	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	_lpvVolume->BindForWriting ();
	_lpvGeometryVolume->BindForWriting ();
}

void LPVRadianceInjectionRenderPass::PostProcessPass (const RenderScene* renderScene,
	const Camera* camera, const RenderSettings& settings,
	const RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	/*
	 * Update voxel volume based on scene bounding box
	*/

	UpdateLPVVolumeBoundingBox (renderScene);

	/*
	 * Set viewport
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();
	glm::ivec2 rsmSize = shadow.resolution / 4;

	GL::Viewport (0, 0, rsmSize.x, rsmSize.y);

	/*
	 * No rendering target
	*/

	// GL::DrawBuffer (GL_NONE);

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Disable (GL_DEPTH_TEST);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Lock post-process shader
	*/

	Pipeline::LockShader (_shaderView);

	/*
	 * Send custom uniforms
	*/

	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("ReflectiveShadowMapVolume")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, _lpvVolume->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (settings, renderLightObject));

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

void LPVRadianceInjectionRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> LPVRadianceInjectionRenderPass::GetCustomAttributes (const RenderSettings& settings,
	const RenderLightObject* renderLightObject) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmResolution;
	PipelineAttribute lightDirection;
	PipelineAttribute injectionBias;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	lightDirection.type = PipelineAttribute::AttrType::ATTR_3F;
	injectionBias.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmResolution.name = "rsmResolution";
	lightDirection.name = "lightDirection";
	injectionBias.name = "injectionBias";

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	glm::vec3 lightDir = renderLightObject->GetTransform ()->GetRotation () * glm::vec3 (0, 0, -1);
	lightDir = glm::normalize (lightDir);

	rsmResolution.value = glm::vec3 (shadow.resolution, 0.0f);
	lightDirection.value = lightDir;
	injectionBias.value.x = settings.lpv_injection_bias;

	attributes.push_back (rsmResolution);
	attributes.push_back (lightDirection);
	attributes.push_back (injectionBias);

	return attributes;
}

void LPVRadianceInjectionRenderPass::UpdateLPVVolumeBoundingBox (const RenderScene* renderScene)
{
	auto& volume = renderScene->GetBoundingBox ();

	glm::vec3 minVertex = volume.minVertex;
	glm::vec3 maxVertex = volume.maxVertex;

	_lpvVolume->UpdateBoundingBox (minVertex, maxVertex);
	_lpvGeometryVolume->UpdateBoundingBox (minVertex, maxVertex);
}

void LPVRadianceInjectionRenderPass::InitLPVVolume (const RenderSettings& settings)
{
	if (!_lpvVolume->Init (settings.lpv_volume_size)) {
		Console::LogError (std::string () +
			"Light propagation volume texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT);
	}

	if (!_lpvGeometryVolume->Init (settings.lpv_volume_size)) {
		Console::LogError (std::string () +
			"Light propagation volume texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT);
	}
}

void LPVRadianceInjectionRenderPass::UpdateLPVVolume (const RenderSettings& settings)
{
	if (_lpvVolume->GetVolumeSize () != settings.lpv_volume_size) {

		/*
		 * Clear voxel volume
		*/

		_lpvGeometryVolume->Clear ();
		_lpvVolume->Clear ();

		/*
		 * Initialize voxel volume
		*/

		InitLPVVolume (settings);
	}
}
