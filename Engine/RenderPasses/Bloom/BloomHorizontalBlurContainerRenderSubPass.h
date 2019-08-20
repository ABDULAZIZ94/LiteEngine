#ifndef BLOOMHORIZONTALBLURCONTAINERRENDERSUBPASS_H
#define BLOOMHORIZONTALBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Blur/HorizontalGaussianBlurContainerRenderSubPass.h"

class ENGINE_API BloomHorizontalBlurContainerRenderSubPass : public HorizontalGaussianBlurContainerRenderSubPass
{
public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
};

#endif