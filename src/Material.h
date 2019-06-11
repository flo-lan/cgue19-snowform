#pragma once

#include <string>

class ShaderProgram;
class CameraComponent;
class MeshRendererComponent;

class Material
{
protected:
    Material(std::string const& name, ShaderProgram* shaderProgram);

public:
    virtual ~Material();

    void Use();

    virtual void PreRender(CameraComponent* camera, MeshRendererComponent* renderer) {}
    virtual void PostRender(CameraComponent* camera, MeshRendererComponent* renderer) {}

protected:
    std::string name;
    ShaderProgram* shaderProgram;
};
