#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Screen.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

CameraComponent::CameraComponent(GameObject* owner) :
    Component::Component(owner),
    viewFrustum(),
    viewFrustumCulling(false),
    transform(GetOwner()->GetComponent<TransformComponent>()),
    projMatrix(1.f),
    viewMatrix(1.f),
    viewProjectionMatrix(1.f),
    orthographicProjectionMatrix(glm::ortho(0.0f, (float)Screen::GetWidth(), 0.0f, (float)Screen::GetHeight()))
{
    GetOwner()->GetScene()->SetCamera(this);
}

CameraComponent::~CameraComponent()
{
    if (GetOwner()->GetScene()->GetCamera() == this)
    {
        GetOwner()->GetScene()->SetCamera(nullptr);
    }
}

void CameraComponent::LateUpdate()
{
    viewFrustum.UpdateViewFrustum(transform->GetPosition(), transform->GetPosition() - transform->GetDirectionBackward(), glm::vec3(0.f, 1.f, 0.f));

    projMatrix = glm::perspective(GetFovInRadians(), GetAspectRatio(), GetNearPlane(), GetFarPlane());
    viewMatrix = glm::inverse(transform->GetModelMatrix());

    viewProjectionMatrix = projMatrix * viewMatrix;
}

glm::vec3 CameraComponent::GetPosition() const
{
    return transform->GetPosition();
}

glm::quat CameraComponent::GetRotation() const
{
    return transform->GetRotationQ();
}
