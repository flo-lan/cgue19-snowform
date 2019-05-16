#include "ThirdPersonControllerComponent.h"
#include "TransformComponent.h"
#include "RigidDynamicComponent.h"
#include "GameScene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Time.h"
#include "glm/ext.hpp"

ThirdPersonControllerComponent::ThirdPersonControllerComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    target(nullptr),
    targetTransform(nullptr),
    targetRigid(nullptr),
    angle(0.f),
    distance(0.f),
    velocity(30.f),
    jumpVelocity(40.f),
    lastMousePositionX(0.f),
    lastMousePositionY(0.f),
    lastMouseScrollValue(0.f)
{
}

void ThirdPersonControllerComponent::OnStart()
{
    if (target = GetOwner()->GetScene()->GetGameObjectById(targetId))
    {
        targetTransform = target->GetComponent<TransformComponent>();
        targetRigid = target->GetComponent<RigidDynamicComponent>();
    }
    else
    {
        fprintf(stderr, "Could not set target of third person controller, because target id '%s' could not be found!\n", targetId.c_str());
    }

    lastMousePositionX = sInputManager.GetMousePositionX();
    lastMousePositionY = sInputManager.GetMousePositionY();
    lastMouseScrollValue = sInputManager.GetMouseScrollValueY();
}

void ThirdPersonControllerComponent::Update()
{
    if (!target ||
        !targetTransform ||
        !targetRigid)
    {
        return;
    }

    angle += glm::radians(sInputManager.GetMousePositionX() - lastMousePositionX) * 0.5f /* Slow down */;

    glm::vec3 direction = glm::vec3(sin(angle), -0.5f, -cos(angle));
    glm::vec3 position = targetTransform->GetPosition() - direction * distance;

    transform->SetPosition(position);
    transform->LookAt(targetTransform);

    if (sInputManager.IsKeyPressed(GLFW_KEY_W))
    {
        glm::vec3 direction = transform->GetDirectionBackward();
        direction.y = 0;
        direction = glm::normalize(direction) * -1;
        targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eIMPULSE);
    }

    if (sInputManager.IsKeyPressed(GLFW_KEY_A))
    {
        glm::vec3 direction = transform->GetDirectionRight();
        direction.y = 0;
        direction = glm::normalize(direction) * -1;
        targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eIMPULSE);
    }

    if (sInputManager.IsKeyPressed(GLFW_KEY_S))
    {
        glm::vec3 direction = transform->GetDirectionBackward();
        direction.y = 0;
        direction = glm::normalize(direction);
        targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eIMPULSE);
    }

    if (sInputManager.IsKeyPressed(GLFW_KEY_D))
    {
        glm::vec3 direction = transform->GetDirectionRight();
        direction.y = 0;
        direction = glm::normalize(direction);
        targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eIMPULSE);
    }

    if (sInputManager.IsKeyPressed(GLFW_KEY_SPACE))
    {
        targetRigid->AddForce(glm::vec3(0.f, jumpVelocity * sTime.GetDeltaTime(), 0.f), physx::PxForceMode::eIMPULSE);
    }

    lastMousePositionX = sInputManager.GetMousePositionX();
    lastMousePositionY = sInputManager.GetMousePositionY();
    lastMouseScrollValue = sInputManager.GetMouseScrollValueY();
}
