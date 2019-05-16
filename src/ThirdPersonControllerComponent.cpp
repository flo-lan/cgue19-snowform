#include "ThirdPersonControllerComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "glm/ext.hpp"

ThirdPersonControllerComponent::ThirdPersonControllerComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    cameraTransform(nullptr),
    lastMousePositionX(0.f),
    lastMousePositionY(0.f),
    lastMouseScrollValue(0.f)
{
}

void ThirdPersonControllerComponent::OnStart()
{
    if (transform && transform->GetChildCount() > 0)
    {
        cameraTransform = transform->GetChild(0);
    }

    rigidDynamic = transform->GetParent()->GetOwner()->GetComponent<RigidDynamicComponent>();

    lastMousePositionX = sInputManager.GetMousePositionX();
    lastMousePositionY = sInputManager.GetMousePositionY();
    lastMouseScrollValue = sInputManager.GetMouseScrollValueY();

    transform->SetIgnoreParentRotation(true);
}

void ThirdPersonControllerComponent::Update()
{
    if (transform)
    {
        float diffX = sInputManager.GetMousePositionX() - lastMousePositionX;
        float localRotationY = -glm::radians(diffX) * 0.5f /* Slow down */;

        transform->Rotate(0.f, localRotationY, 0.f);
    }

    if (cameraTransform)
    {
        float diffMouseScrollValue = sInputManager.GetMouseScrollValueY() - lastMouseScrollValue;

        cameraTransform->SetLocalPositionZ(cameraTransform->GetLocalPositionZ() - diffMouseScrollValue);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_W))
    {
        glm::vec3 direction = cameraTransform->GetDirectionBackward();
        direction.y = 0;
        direction = glm::normalize(direction) * -1;
        rigidDynamic->AddForce(direction * velocity, physx::PxForceMode::eFORCE);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_A))
    {
        glm::vec3 direction = cameraTransform->GetDirectionRight();
        direction.y = 0;
        direction = glm::normalize(direction) * -1;
        rigidDynamic->AddForce(direction * velocity, physx::PxForceMode::eFORCE);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_S))
    {
        glm::vec3 direction = cameraTransform->GetDirectionBackward();
        direction.y = 0;
        direction = glm::normalize(direction);
        rigidDynamic->AddForce(direction * velocity, physx::PxForceMode::eFORCE);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_D))
    {
        glm::vec3 direction = cameraTransform->GetDirectionRight();
        direction.y = 0;
        direction = glm::normalize(direction);
        rigidDynamic->AddForce(direction * velocity, physx::PxForceMode::eFORCE);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_SPACE))
    {
        rigidDynamic->AddForce(glm::vec3(0.f, jumpVelocity, 0.f), physx::PxForceMode::eFORCE);
    }

    lastMousePositionX = sInputManager.GetMousePositionX();
    lastMousePositionY = sInputManager.GetMousePositionY();
    lastMouseScrollValue = sInputManager.GetMouseScrollValueY();
}
