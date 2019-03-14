#pragma once

#include <string>

class ShaderProgram;
class MeshRendererComponent;

class Material
{
protected:
    Material(std::string const& name, ShaderProgram* shaderProgram);

public:
    virtual ~Material();

    void Use();

    virtual void SetUniforms(MeshRendererComponent* renderer) {}

protected:
    std::string name;
    ShaderProgram* shaderProgram;
};
