#ifndef GUIRENDERPASS_H
#define GUIRENDERPASS_H

#include "Renderer/RenderPassI.h"

class ENGINE_API GUIRenderPass : public RenderPassI
{
	DECLARE_RENDER_PASS(GUIRenderPass)

protected:
	std::string _shaderName;
	unsigned int _VAO_ID;
	unsigned int _VBO_ID;
	unsigned int _IBO_ID;

public:
	GUIRenderPass ();
	~GUIRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
protected:
	void StartEditorPass ();
	void EditorPass (const RenderSettings& settings);
	void EndEditorPass ();

	std::vector<PipelineAttribute> GetCustomAttributes ();

	void LockShader ();

	void Clear ();
};

#endif