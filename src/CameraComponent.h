#pragma once

#include "Component.h"

#include <glm\glm.hpp>

class TransformComponent;

class CameraComponent : public Component
{
public:
    CameraComponent(GameObject* owner);
    virtual ~CameraComponent();

    virtual void OnStart();
    virtual void LateUpdate();

    void SetFov(float value) { fov = value; }
    void SetAspectRatio(float value) { aspectRatio = value; }
    void SetNearPlane(float value) { nearPlane = value; }
    void SetFarPlane(float value) { farPlane = value; }

    glm::vec3 const& GetPosition();
    glm::mat4 const& GetViewProjectionMatrix() const { return viewProjectionMatrix; }

private:
    TransformComponent* transform;
    glm::mat4 viewProjectionMatrix;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
};
