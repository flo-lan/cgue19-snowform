#pragma once

#include "Material.h"
#include <glm\glm.hpp>

class Texture2D;

class ParticleBillboardMaterial : public Material
{
public:
    ParticleBillboardMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~ParticleBillboardMaterial();

    virtual void SetUniforms(CameraComponent* camera, MeshRendererComponent* renderer);

    void SetDiffuseTexture(Texture2D* diffuseTexture) { this->diffuseTexture = diffuseTexture; }
    void SetDiffuseColor(glm::vec3 diffuseColor) { this->diffuseColor = diffuseColor; }

private:
    Texture2D* defaultTexture;
    Texture2D* diffuseTexture;
    glm::vec3 diffuseColor;
};
