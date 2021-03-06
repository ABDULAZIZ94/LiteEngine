#ifndef VOXELBORDERRENDERPASS_H
#define VOXELBORDERRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class VoxelBorderRenderPass : public RenderPassI
{
	DECLARE_RENDER_PASS(VoxelBorderRenderPass)

protected:
	Resource<ShaderView> _shaderView;
	bool _firstTime;

public:
	VoxelBorderRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartVoxelBordering ();
	void BorderVoxelVolume (const RenderSettings&, RenderVolumeCollection*);
	void EndVoxelBordering ();
};

#endif