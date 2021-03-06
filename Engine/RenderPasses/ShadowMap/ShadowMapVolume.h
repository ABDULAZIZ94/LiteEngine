#ifndef SHADOWMAPVOLUME_H
#define SHADOWMAPVOLUME_H

#include "RenderPasses/FrameBuffer2DVolume.h"

#define SHADOW_MAP_FBO_NOT_INIT 330

class ShadowMapVolume : public FrameBuffer2DVolume
{
public:
	bool Init (const glm::ivec2& size);

	void BindForReading ();
	void BindForWriting ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif