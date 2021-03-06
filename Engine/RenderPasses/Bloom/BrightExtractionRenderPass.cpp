#include "BrightExtractionRenderPass.h"

#include "RenderPasses/Blur/BlurMapVolume.h"

bool BrightExtractionRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return settings.bloom_enabled;
}

std::string BrightExtractionRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Bloom/brightExtractionFragment.glsl";
}

std::string BrightExtractionRenderPass::GetPostProcessVolumeName () const
{
	return "BlurMapVolume";
}

glm::ivec2 BrightExtractionRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.bloom_scale);
}

PostProcessMapVolume* BrightExtractionRenderPass::CreatePostProcessVolume () const
{
	BlurMapVolume* blurMapVolume = new BlurMapVolume ();

	return blurMapVolume;
}

std::vector<PipelineAttribute> BrightExtractionRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach bloom attributes to pipeline
	*/

	PipelineAttribute bloomResolution;
	PipelineAttribute bloomThreshold;

	bloomResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	bloomThreshold.type = PipelineAttribute::AttrType::ATTR_1F;

	bloomResolution.name = "bloomResolution";
	bloomThreshold.name = "bloomThreshold";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.bloom_scale);

	bloomResolution.value = glm::vec3 (resolution, 0.0f);
	bloomThreshold.value.x = settings.bloom_threshold;

	attributes.push_back (bloomResolution);
	attributes.push_back (bloomThreshold);

	return attributes;
}
