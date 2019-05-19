#pragma once

#include "Material.h"
#include <glm\glm.hpp>

class ShadowMapMaterial : public Material
{
public:
    ShadowMapMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~ShadowMapMaterial();

    virtual void SetUniforms(MeshRendererComponent* renderer);

    void SetLightSpaceMatrix(glm::mat4 const& lightSpaceMatrix) { this->lightSpaceMatrix = lightSpaceMatrix; }

private:
    glm::mat4 lightSpaceMatrix;
};
