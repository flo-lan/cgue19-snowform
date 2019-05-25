#pragma once

#include "Material.h"
#include <glm\glm.hpp>

class Texture2D;

class ImageMaterial : public Material
{
public:
    ImageMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~ImageMaterial();

    virtual void SetUniforms(CameraComponent* camera, MeshRendererComponent* renderer);

    void SetImageTexture(Texture2D* value) { imageTexture = value; }
    void SetImageColor(glm::vec4 value) { imageColor = value; }

protected:
    Texture2D* defaultTexture;
    Texture2D* imageTexture;
    glm::vec4 imageColor;
};
