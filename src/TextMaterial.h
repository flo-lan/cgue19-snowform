#pragma once

#include "Material.h"
#include <glm\glm.hpp>

class Texture2D;

class TextMaterial : public Material
{
public:
    TextMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~TextMaterial();

    virtual void SetUniforms(CameraComponent* camera, MeshRendererComponent* renderer);

    void SetAtlasTexture(Texture2D* value) { atlasTexture = value; }
    void SetTextColor(glm::vec4 value) { textColor = value; }

private:
    Texture2D* defaultTexture;
    Texture2D* atlasTexture;
    glm::vec4 textColor;
};
