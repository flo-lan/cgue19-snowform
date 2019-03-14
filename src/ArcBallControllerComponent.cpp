#include "ArcBallControllerComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "InputManager.h"

#include <glm/gtc/constants.hpp>

ArcBallControllerComponent::ArcBallControllerComponent(GameObject* owner) :
    Component::Component(owner),
    transform(nullptr),
    cameraTransform(nullptr),
    lastMousePositionX(0.f),
    lastMousePositionY(0.f),
    lastMouseScrollValue(0.f)
{
}

void ArcBallControllerComponent::OnStart()
{
    transform = GetOwner()->GetComponent<TransformComponent>();

    if (transform && transform->GetChildCount() > 0)
    {
        cameraTransform = transform->GetChild(0);
    }

    lastMousePositionX = sInputManager.GetMousePositionX();
    lastMousePositionY = sInputManager.GetMousePositionY();
    lastMouseScrollValue = sInputManager.GetMouseScrollValueY();
}

void ArcBallControllerComponent::Update()
{
    if (sInputManager.IsLeftMouseButtonPressed() && transform)
    {
        float diffX = sInputManager.GetMousePositionX() - lastMousePositionX;
        float diffY = sInputManager.GetMousePositionY() - lastMousePositionY;

        float localRotationX = transform->GetLocalRotationX() - glm::radians(diffY) * 0.5f /* Slow down */;
        float localRotationY = transform->GetLocalRotationY() - glm::radians(diffX) * 0.5f /* Slow down */;

        // Clamp vertical rotation between -90° and 90°
        localRotationX = glm::clamp(localRotationX, -glm::half_pi<float>(), glm::half_pi<float>());

        transform->SetLocalRotationX(localRotationX);
        transform->SetLocalRotationY(localRotationY);
    }

    if (sInputManager.IsRightMouseButtonPressed() && transform && cameraTransform)
    {
        float diffMousePositionX = sInputManager.GetMousePositionX() - lastMousePositionX;
        float diffMousePositionY = sInputManager.GetMousePositionY() - lastMousePositionY;

        transform->SetLocalPosition(transform->GetLocalPosition() +
           -transform->GetDirectionRight() * diffMousePositionX * 0.01f /* Slow down */ +
            transform->GetDirectionUp()    * diffMousePositionY * 0.01f /* Slow down */
        );
    }

    if (cameraTransform)
    {
        float diffMouseScrollValue = sInputManager.GetMouseScrollValueY() - lastMouseScrollValue;

        cameraTransform->SetLocalPositionZ(cameraTransform->GetLocalPositionZ() - diffMouseScrollValue);
    }

    lastMousePositionX = sInputManager.GetMousePositionX();
    lastMousePositionY = sInputManager.GetMousePositionY();
    lastMouseScrollValue = sInputManager.GetMouseScrollValueY();
}
