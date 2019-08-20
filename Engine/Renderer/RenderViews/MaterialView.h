#ifndef MATERIALVIEW_H
#define MATERIALVIEW_H

#include "Core/Interfaces/Object.h"

#include <glm/vec3.hpp>

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/TextureView.h"

class MaterialView : public Object
{
public:
	std::string name;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
	float transparency;
	int illum;
	std::pair<int, int> blending;
	Resource<TextureView> ambientTexture;
	Resource<TextureView> diffuseTexture;
	Resource<TextureView> specularTexture;
	Resource<TextureView> specularHighlight;
	Resource<TextureView> alphaTexture;
	Resource<TextureView> bumpTexture;
	Resource<TextureView> cubeTexture;
	std::string shaderName;

	MaterialView ();
};

#endif