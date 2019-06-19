#pragma once

#include "Component.h"
#include "ViewFrustum.h"
#include <glm\glm.hpp>

class TransformComponent;

class CameraComponent : public Component
{
public:
    CameraComponent(GameObject* owner);
    virtual ~CameraComponent();

    virtual void LateUpdate();

    void SetViewFrustumCulling(bool viewFrustumCulling) { this->viewFrustumCulling = viewFrustumCulling; }
    void SetFovInDegrees(float value) { viewFrustum.SetFovInDegrees(value); }
    void SetAspectRatio(float value) { viewFrustum.SetAspectRatio(value); }
    void SetNearPlane(float value) { viewFrustum.SetNearPlane(value); }
    void SetFarPlane(float value) { viewFrustum.SetFarPlane(value); }

    bool IsViewFrustumCullingEnabled() const { return viewFrustumCulling; }
    float GetFovInRadians() const { return viewFrustum.GetFovInRadians(); }
    float GetAspectRatio() const { return viewFrustum.GetAspectRatio(); }
    float GetNearPlane() const { return viewFrustum.GetNearPlane(); }
    float GetFarPlane() const { return viewFrustum.GetFarPlane(); }

    TransformComponent* GetTransform() const { return transform; }
    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::mat4 const& GetProjMatrix() const { return projMatrix; }
    glm::mat4 const& GetViewMatrix() const { return viewMatrix; }
    glm::mat4 const& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
    glm::mat4 const& GetOrthographicProjectionMatrix() const { return orthographicProjectionMatrix; }

    ViewFrustumObjectLocation BoundsInFrustum(Bounds const& bounds) const { return viewFrustum.BoundsInFrustum(bounds); }

    int ObjectsRendered;
    int ObjectsCulled;

private:
    ViewFrustum viewFrustum;
    bool viewFrustumCulling;
    TransformComponent* transform;
    glm::mat4 projMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 orthographicProjectionMatrix;
};
