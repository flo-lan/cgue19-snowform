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

    float GetFOV() const { return fov; }
    float GetNearPlane() const { return nearPlane; }
    float GetFarPlane() const { return farPlane; }

    void SetFov(float value) { fov = glm::radians(value); UpdatePerspectiveProjectionMatrix(); }
    void SetAspectRatio(float value) { aspectRatio = value; UpdatePerspectiveProjectionMatrix(); }
    void SetNearPlane(float value) { nearPlane = value; UpdatePerspectiveProjectionMatrix(); }
    void SetFarPlane(float value) { farPlane = value; UpdatePerspectiveProjectionMatrix(); }

    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetDirection() const;
    glm::mat4 const& GetModelMatrix() const;
    glm::mat4 const& GetPersProjectionMatrix() const { return persProjectionMatrix; }
    glm::mat4 const& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
    glm::mat4 const& GetOrthProjectionMatrix() const { return orthProjectionMatrix; }

private:
    void UpdatePerspectiveProjectionMatrix();

    TransformComponent* transform;
    glm::mat4 persProjectionMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 orthProjectionMatrix;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
};
