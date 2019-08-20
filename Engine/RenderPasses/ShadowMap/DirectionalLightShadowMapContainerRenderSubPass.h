#ifndef DIRECTIONALLIGHTSHADOWMAPCONTAINERRENDERSUBPASS_H
#define DIRECTIONALLIGHTSHADOWMAPCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"
#include "Core/Observer/ObserverI.h"

#include "RenderPasses/ShadowMap/CascadedShadowMapDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"
#include "Cameras/OrthographicCamera.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class ENGINE_API DirectionalLightShadowMapContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	CascadedShadowMapDirectionalLightVolume* _volume;

	std::size_t _cascades;
	glm::ivec2 _resolution;

public:
	DirectionalLightShadowMapContainerRenderSubPass ();
	~DirectionalLightShadowMapContainerRenderSubPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void ShadowMapPass (const RenderScene* renderScene, const Camera* camera, const RenderLightObject* renderLightObject);
	void EndShadowMapPass ();

	void UpdateCascadeLevelsLimits (const Camera* camera);
	void SendLightCamera (Camera* lightCamera);
	void UpdateLightCameras (const Camera* viewCamera, const RenderLightObject* renderLightObject);
	void Render (const RenderScene* renderScene, OrthographicCamera* lightCamera);
	void LockShader (int sceneLayers);

	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	virtual void InitSettings ();
	virtual void ClearSettings ();

	virtual void InitShadowMapVolume ();
};

#endif