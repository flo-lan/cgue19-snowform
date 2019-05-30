#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Screen.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

CameraComponent::CameraComponent(GameObject* owner) :
    Component::Component(owner),
    transform(GetOwner()->GetComponent<TransformComponent>()),
    persProjectionMatrix(1.f),
    viewProjectionMatrix(1.f),
    orthProjectionMatrix(glm::ortho(0.0f, (float)Screen::GetWidth(), 0.0f, (float)Screen::GetHeight())),
    fov(glm::radians(60.f)),
    aspectRatio(Screen::GetWidth() / (float)Screen::GetHeight()),
    nearPlane(0.1f),
    farPlane(100.f)
{
    UpdatePerspectiveProjectionMatrix();

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
    viewProjectionMatrix = persProjectionMatrix * glm::inverse(transform->GetModelMatrix());
}

glm::vec3 CameraComponent::GetPosition() const
{
    return transform->GetPosition();
}

glm::quat CameraComponent::GetRotation() const
{
    return transform->GetRotationQ();
}

glm::vec3 CameraComponent::GetDirection() const
{
    return -transform->GetDirectionBackward();
}

glm::mat4 const& CameraComponent::GetModelMatrix() const
{
    return transform->GetModelMatrix();
}

void CameraComponent::UpdatePerspectiveProjectionMatrix()
{
    persProjectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
