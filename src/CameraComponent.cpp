#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

CameraComponent* CameraComponent::main = nullptr;

CameraComponent::CameraComponent(GameObject* owner) :
    Component::Component(owner),
    transform(nullptr),
    viewProjectionMatrix(1.f),
    fov(60.f),
    aspectRatio(16.f / 9.f),
    nearPlane(0.1f),
    farPlane(100.f)
{
    if (main == nullptr)
    {
        main = this;
    }
}

CameraComponent::~CameraComponent()
{
    if (main == this)
    {
        main = nullptr;
    }
}

void CameraComponent::OnStart()
{
    transform = GetOwner()->GetComponent<TransformComponent>();
}

void CameraComponent::LateUpdate()
{
    viewProjectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane) /* Project Matrix */ *
        (transform ? glm::inverse(transform->GetModelMatrix()) : glm::mat4(1.f)) /* View Matrix */;
}

glm::vec3 CameraComponent::GetPosition() const
{
    return transform ? transform->GetPosition() : glm::vec3(0.f);
}
