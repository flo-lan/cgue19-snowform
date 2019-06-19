#include "ThirdPersonControllerComponent.h"
#include "TransformComponent.h"
#include "RigidDynamicComponent.h"
#include "PhysicsEngine.h"
#include "GameScene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Time.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <algorithm>

ThirdPersonControllerComponent::ThirdPersonControllerComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    target(nullptr),
    targetTransform(nullptr),
    targetRigid(nullptr),
    angle(0.f),
    distance(10.f),
    minDistance(5.f),
    maxDistance(25.f),
    scrollSpeed(30.f),
    velocity(30.f),
    jumpVelocity(20.f),
    jumpTimer(0.f),
    lastMousePositionX(0.f),
    lastMousePositionY(0.f),
    lastMouseScrollValue(0.f),
    remainingMouseScroll(0.f),
    isGrounded(false)
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
    static const glm::vec3 Down = glm::vec3(0.f, -1.f, 0.f);

    if (!target ||
        !targetTransform ||
        !targetRigid)
    {
        return;
    }

    remainingMouseScroll += (lastMouseScrollValue - sInputManager.GetMouseScrollValueY());

    if (std::abs(remainingMouseScroll) > 0.01f)
    {
        float scrollDistance = scrollSpeed * sTime.GetDeltaTime();

        if (std::abs(remainingMouseScroll) < std::abs(scrollDistance))
        {
            scrollDistance = remainingMouseScroll;
        }
        else if (remainingMouseScroll < 0.f)
        {
            scrollDistance *= -1.f;
        }

        remainingMouseScroll -= scrollDistance;

        distance += scrollDistance;

        if (distance < minDistance)
        {
            distance = minDistance;
            remainingMouseScroll = 0.f;
        }
        else if (distance > maxDistance)
        {
            distance = maxDistance;
            remainingMouseScroll = 0.f;
        }
    }

    angle += glm::radians(sInputManager.GetMousePositionX() - lastMousePositionX) * 0.5f /* Slow down */;

    glm::vec3 direction = glm::vec3(sin(angle), -0.5f, -cos(angle));
    glm::vec3 position = targetTransform->GetPosition() - direction * distance;

    transform->SetPosition(position);
    transform->LookAt(targetTransform);

    if (jumpTimer > 0.f)
    {
        jumpTimer -= sTime.GetDeltaTime();

        if (jumpTimer <= 0.f)
        {
            jumpTimer = 0.f;
        }
    }

    RaycastHit hitInfo;

    isGrounded = sPhysicsEngine.Raycast(targetTransform->GetPosition(), Down, 5.0f, hitInfo, RAYCAST_LAYER_DEFAULT) &&
        hitInfo.Distance < 1.f;

    if (isGrounded)
    {
        if (sInputManager.IsKeyPressed(GLFW_KEY_W))
        {
            glm::vec3 direction = transform->GetDirectionBackward();
            glm::vec3 torqueDirection = transform->GetDirectionRight() * -1.f;
            direction.y = 0;
            direction = glm::normalize(direction) * -1;
            torqueDirection.y = 0;
            torqueDirection = glm::normalize(torqueDirection);
            targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eVELOCITY_CHANGE);
            targetRigid->AddTorque(torqueDirection * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eVELOCITY_CHANGE);
        }

        if (sInputManager.IsKeyPressed(GLFW_KEY_A))
        {
            glm::vec3 direction = transform->GetDirectionRight();
            glm::vec3 torqueDirection = transform->GetDirectionBackward();
            direction.y = 0;
            direction = glm::normalize(direction) * -1;
            torqueDirection.y = 0;
            torqueDirection = glm::normalize(torqueDirection);
            targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eVELOCITY_CHANGE);
            targetRigid->AddTorque(torqueDirection * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eVELOCITY_CHANGE);
        }

        if (sInputManager.IsKeyPressed(GLFW_KEY_S))
        {
            glm::vec3 direction = transform->GetDirectionBackward();
            glm::vec3 torqueDirection = transform->GetDirectionRight();
            direction.y = 0;
            direction = glm::normalize(direction);
            torqueDirection.y = 0;
            torqueDirection = glm::normalize(torqueDirection);
            targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eVELOCITY_CHANGE);
            targetRigid->AddTorque(torqueDirection * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eVELOCITY_CHANGE);
        }

        if (sInputManager.IsKeyPressed(GLFW_KEY_D))
        {
            glm::vec3 direction = transform->GetDirectionRight();
            glm::vec3 torqueDirection = transform->GetDirectionBackward() * -1.f;
            direction.y = 0;
            direction = glm::normalize(direction);
            torqueDirection.y = 0;
            torqueDirection = glm::normalize(torqueDirection);
            targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eVELOCITY_CHANGE);
            targetRigid->AddTorque(torqueDirection * velocity * sTime.GetDeltaTime(), physx::PxForceMode::eVELOCITY_CHANGE);
        }

        if (sInputManager.IsKeyPressed(GLFW_KEY_SPACE) && jumpTimer == 0.f)
        {
            jumpTimer = 0.5f; // max 2 jumps / second

            targetRigid->AddForce(glm::vec3(0.f, jumpVelocity, 0.f), physx::PxForceMode::eVELOCITY_CHANGE);
        }
    }
    else
    {
        if (sInputManager.IsKeyPressed(GLFW_KEY_W))
        {
            glm::vec3 direction = transform->GetDirectionBackward();
            glm::vec3 torqueDirection = transform->GetDirectionRight() * -1.f;
            direction.y = 0;
            direction = glm::normalize(direction) * -1;
            torqueDirection.y = 0;
            torqueDirection = glm::normalize(torqueDirection);
            targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime() * 0.1f, physx::PxForceMode::eVELOCITY_CHANGE);
            targetRigid->AddTorque(torqueDirection * velocity * sTime.GetDeltaTime() * 0.1f, physx::PxForceMode::eVELOCITY_CHANGE);
        }

        if (sInputManager.IsKeyPressed(GLFW_KEY_A))
        {
            glm::vec3 direction = transform->GetDirectionRight();
            glm::vec3 torqueDirection = transform->GetDirectionBackward();
            direction.y = 0;
            direction = glm::normalize(direction) * -1;
            torqueDirection.y = 0;
            torqueDirection = glm::normalize(torqueDirection);
            targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime() * 0.1f, physx::PxForceMode::eVELOCITY_CHANGE);
            targetRigid->AddTorque(torqueDirection * velocity * sTime.GetDeltaTime() * 0.1f, physx::PxForceMode::eVELOCITY_CHANGE);
        }

        if (sInputManager.IsKeyPressed(GLFW_KEY_S))
        {
            glm::vec3 direction = transform->GetDirectionBackward();
            glm::vec3 torqueDirection = transform->GetDirectionRight();
            direction.y = 0;
            direction = glm::normalize(direction);
            torqueDirection.y = 0;
            torqueDirection = glm::normalize(torqueDirection);
            targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime() * 0.1f, physx::PxForceMode::eVELOCITY_CHANGE);
            targetRigid->AddTorque(torqueDirection * velocity * sTime.GetDeltaTime() * 0.1f, physx::PxForceMode::eVELOCITY_CHANGE);
        }

        if (sInputManager.IsKeyPressed(GLFW_KEY_D))
        {
            glm::vec3 direction = transform->GetDirectionRight();
            glm::vec3 torqueDirection = transform->GetDirectionBackward() * -1.f;
            direction.y = 0;
            direction = glm::normalize(direction);
            torqueDirection.y = 0;
            torqueDirection = glm::normalize(torqueDirection);
            targetRigid->AddForce(direction * velocity * sTime.GetDeltaTime() * 0.1f, physx::PxForceMode::eVELOCITY_CHANGE);
            targetRigid->AddTorque(torqueDirection * velocity * sTime.GetDeltaTime() * 0.1f, physx::PxForceMode::eVELOCITY_CHANGE);
        }
    }

    lastMousePositionX = sInputManager.GetMousePositionX();
    lastMousePositionY = sInputManager.GetMousePositionY();
    lastMouseScrollValue = sInputManager.GetMouseScrollValueY();
}