#include "LightMapDirectionalLightContainerRenderSubPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

LightMapDirectionalLightContainerRenderSubPass::LightMapDirectionalLightContainerRenderSubPass () :
	_shaderName ("LIGHT_MAP_DIRECTIONAL_LIGHT")
{

}

void LightMapDirectionalLightContainerRenderSubPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/LightMap/lightMapDirVolLightFragment.glsl");
}

void LightMapDirectionalLightContainerRenderSubPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void LightMapDirectionalLightContainerRenderSubPass::LockShader (const RenderLightObject* renderLightObject)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock general shader for directional light
	*/

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));
}

std::vector<PipelineAttribute> LightMapDirectionalLightContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}
