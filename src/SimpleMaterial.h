#pragma once

#include "Material.h"
#include <glm\glm.hpp>

class SimpleMaterial : public Material
{
public:
    SimpleMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~SimpleMaterial();

    virtual void SetUniforms(MeshRendererComponent* renderer);

    void SetColor(glm::vec3 value) { color = value; }

private:
    glm::vec3 color;
};
