#pragma once

#include "Component.h"

#include <glm\glm.hpp>

class ShaderProgram;
class CameraComponent;
class TransformComponent;

class TeapotRendererComponent : public Component
{
public:
    TeapotRendererComponent(GameObject* owner);
    virtual ~TeapotRendererComponent();

    virtual void OnStart();
    virtual void Render();

    void SetCamera(CameraComponent* value) { camera = value; }
    void SetShaderProgram(ShaderProgram* value) { shaderProgram = value; }
    void SetColor(glm::vec3 value) { color = value; }

private:
    CameraComponent* camera;
    TransformComponent* transform;
    ShaderProgram* shaderProgram;
    glm::vec3 color;
};
