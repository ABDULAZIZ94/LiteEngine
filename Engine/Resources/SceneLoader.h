#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <glm/vec3.hpp>
#include <string>

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Utils/Color/Color.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/Transform.h"
#include "SceneNodes/GameObject.h"

class SceneLoader
{
public:
	static SceneLoader& Instance ();

	Scene* Load (const std::string& filename);
private:
	SceneLoader ();

	void ProcessLight (TiXmlElement* xmlElem, Scene* scene);
	void ProcessSkybox (TiXmlElement* xmlElem, Scene* scene);
	void ProcessSceneObject (TiXmlElement* xmlElem, Scene* scene);
	void ProcessParticleSystem (TiXmlElement* xmlElem, Scene* scene);

	void ProcessTransform (TiXmlElement* xmlElem, Scene* scene, SceneObject* obj);
	glm::vec3 GetPosition (TiXmlElement* xmlElem);
	glm::quat GetRotation (TiXmlElement* xmlElem);
	glm::vec3 GetScale (TiXmlElement* xmlElem);

	void ProcessComponents (TiXmlElement* xmlElem, GameObject* gameObject);
	void ProcessComponent (TiXmlElement* xmlElem, GameObject* gameObject);

	void ProcessRigidbody (TiXmlElement* xmlElem, GameObject* object);

	void ProcessAudioSource (TiXmlElement* xmlElem, GameObject* object);

	void ProcessCollider (TiXmlElement* xmlElem, Rigidbody* rigidbody);

	void ProcessAudioClip (TiXmlElement* xmlElem, AudioSource* audioSource);
};

#endif