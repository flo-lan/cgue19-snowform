#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Screen.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

CameraComponent* CameraComponent::main = nullptr;

CameraComponent::CameraComponent(GameObject* owner) :
    Component::Component(owner),
    transform(GetOwner()->GetComponent<TransformComponent>()),
    viewProjectionMatrix(1.f),
    orthographicProjectionMatrix(glm::ortho(0.0f, (float)Screen::GetWidth(), 0.0f, (float)Screen::GetHeight())),
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

void CameraComponent::LateUpdate()
{
    glm::mat4 projMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
    glm::mat4 viewMatrix = glm::inverse(transform->GetModelMatrix());

    viewProjectionMatrix = projMatrix * viewMatrix;
}

glm::vec3 CameraComponent::GetPosition() const
{
    return transform ? transform->GetPosition() : glm::vec3(0.f);
}
