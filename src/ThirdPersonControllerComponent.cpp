#include "ThirdPersonControllerComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "glm/ext.hpp"

ThirdPersonControllerComponent::ThirdPersonControllerComponent(GameObject* owner) :
    Component::Component(owner),
    transform(nullptr),
    cameraTransform(nullptr),
    lastMousePositionX(0.f),
    lastMousePositionY(0.f),
    lastMouseScrollValue(0.f)
{
}

void ThirdPersonControllerComponent::OnStart()
{
    transform = GetOwner()->GetComponent<TransformComponent>();

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
        float diffY = sInputManager.GetMousePositionY() - lastMousePositionY;

        float localRotationX = -glm::radians(diffY) * 0.5f /* Slow down */;
        float localRotationY = -glm::radians(diffX) * 0.5f /* Slow down */;

        transform->Rotate(localRotationX, localRotationY, 0.f);
    }

    if (cameraTransform)
    {
        float diffMouseScrollValue = sInputManager.GetMouseScrollValueY() - lastMouseScrollValue;

        cameraTransform->SetLocalPositionZ(cameraTransform->GetLocalPositionZ() - diffMouseScrollValue);
    }
    float velocity = 20.f;
    float jumpVelocity = 20.f;
    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_W))
    {
        rigidDynamic->AddForce(glm::vec3(0.f, 0.f, -velocity), physx::PxForceMode::eFORCE);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_A))
    {
        rigidDynamic->AddForce(glm::vec3(-velocity, 0.f, 0.f), physx::PxForceMode::eFORCE);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_S))
    {
        rigidDynamic->AddForce(glm::vec3(0.f, 0.f, velocity), physx::PxForceMode::eFORCE);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_D))
    {
        rigidDynamic->AddForce(glm::vec3(velocity, 0.f, 0.f), physx::PxForceMode::eFORCE);
    }

    if (rigidDynamic && sInputManager.IsKeyPressed(GLFW_KEY_SPACE))
    {
        rigidDynamic->AddForce(glm::vec3(0.f, jumpVelocity, 0.f), physx::PxForceMode::eFORCE);
    }

    lastMousePositionX = sInputManager.GetMousePositionX();
    lastMousePositionY = sInputManager.GetMousePositionY();
    lastMouseScrollValue = sInputManager.GetMouseScrollValueY();
}
