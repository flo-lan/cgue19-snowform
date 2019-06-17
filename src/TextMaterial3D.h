#pragma once

#include "Material.h"
#include <glm\glm.hpp>

class Texture2D;

class TextMaterial3D : public Material
{
public:
    TextMaterial3D(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~TextMaterial3D();

    virtual void PreRender(CameraComponent* camera, MeshRendererComponent* renderer);
    virtual void PostRender(CameraComponent* camera, MeshRendererComponent* renderer);

    void SetAtlasTexture(Texture2D* value) { atlasTexture = value; }
    void SetTextColor(glm::vec4 value) { textColor = value; }

private:
    Texture2D* defaultTexture;
    Texture2D* atlasTexture;
    glm::vec4 textColor;
};
