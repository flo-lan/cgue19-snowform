#pragma once

#include "Component.h"

#include <glm\glm.hpp>

class TransformComponent;

class CameraComponent : public Component
{
public:
    CameraComponent(GameObject* owner);
    virtual ~CameraComponent();

    virtual void LateUpdate();

    void SetFov(float value) { fov = value; }
    void SetAspectRatio(float value) { aspectRatio = value; }
    void SetNearPlane(float value) { nearPlane = value; }
    void SetFarPlane(float value) { farPlane = value; }

    glm::vec3 GetPosition() const;
    glm::mat4 const& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
    glm::mat4 const& GetOrthographicProjectionMatrix() const { return orthographicProjectionMatrix; }

    static CameraComponent* GetMainCameraComponent() { return main; }

private:
    TransformComponent* transform;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 orthographicProjectionMatrix;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    static CameraComponent* main;
};
