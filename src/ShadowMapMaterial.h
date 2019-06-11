#pragma once

#include "Material.h"
#include <glm\glm.hpp>

class ShadowMapMaterial : public Material
{
public:
    ShadowMapMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~ShadowMapMaterial();

    virtual void PreRender(CameraComponent* camera, MeshRendererComponent* renderer);

    void SetShadowMapProjection(glm::mat4 const& shadowMapProjection) { this->shadowMapProjection = shadowMapProjection; }

private:
    glm::mat4 shadowMapProjection;
};
