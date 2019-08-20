#ifndef DEFERREDBLITRENDERPASS_H
#define DEFERREDBLITRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

class ENGINE_API DeferredBlitRenderPass : public RenderPassI
{
public:
	virtual ~DeferredBlitRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void EndDrawing (FrameBuffer2DVolume* frameBufferVolume, FrameBuffer2DVolume* resultFramebuffer,
		const RenderSettings& settings);
};

#endif