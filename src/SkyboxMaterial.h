#pragma once

#include "Material.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

class Cubemap;

class SkyboxMaterial : public Material
{
public:
    SkyboxMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~SkyboxMaterial();

    virtual void PreRender(CameraComponent* camera, MeshRendererComponent* renderer);
    virtual void PostRender(CameraComponent* camera, MeshRendererComponent* renderer);

    void SetCubemap(Cubemap* cubemap) { this->cubemap = cubemap; }

private:
    Cubemap* cubemap;
    GLint prevCullFaceMode;
    GLint prevDepthFuncMode;
};
