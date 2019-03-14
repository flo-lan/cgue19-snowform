#pragma once

#include "Material.h"
#include <glm\glm.hpp>
#include <vector>

class Texture2D;

class PhongGouraudMaterial : public Material
{
public:
    PhongGouraudMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~PhongGouraudMaterial();

    virtual void SetUniforms(MeshRendererComponent* renderer);

    void SetDiffuseColor(glm::vec3 value) { diffuseColor = value; }
    void SetSpecularColor(glm::vec3 value) { specularColor = value; }

    void SetDiffuseTexture(Texture2D* value) { diffuseTexture = value; }
    void SetSpecularTexture(Texture2D* value) { specularTexture = value; }

    void SetAmbientReflectionConstant(float value) { ambientReflectionConstant = value; }
    void SetDiffuseReflectionConstant(float value) { diffuseReflectionConstant = value; }
    void SetSpecularReflectionConstant(float value) { specularReflectionConstant = value; }

    void SetShininess(float value) { shininess = value; }

private:
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    Texture2D* defaultTexture;
    Texture2D* diffuseTexture;
    Texture2D* specularTexture;

    float ambientReflectionConstant;
    float diffuseReflectionConstant;
    float specularReflectionConstant;

    float shininess;
};
