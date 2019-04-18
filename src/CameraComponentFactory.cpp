#include "CameraComponentFactory.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "Settings.h"
#include "tinyxml2.h"

CameraComponentFactory::CameraComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void CameraComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    CameraComponent* cameraComponent = gameObject->AttachComponent<CameraComponent>();

    const int width = sSettings.getWindowWidth();
    const int height = sSettings.getWindowHeight();

    cameraComponent->SetAspectRatio((float)width / height);

    if (element->Attribute("fov"))
    {
        std::string fovValue = std::string(element->Attribute("fov"));
        cameraComponent->SetFov((float)atof(fovValue.c_str()));
    }
    else
    {
        cameraComponent->SetFov(sSettings.getCameraFov());
    }

    if (element->Attribute("near"))
    {
        std::string nearValue = std::string(element->Attribute("near"));
        cameraComponent->SetNearPlane((float)atof(nearValue.c_str()));
    }
    else
    {
        cameraComponent->SetNearPlane(sSettings.getCameraNear());
    }

    if (element->Attribute("far"))
    {
        std::string farValue = std::string(element->Attribute("far"));
        cameraComponent->SetFarPlane((float)atof(farValue.c_str()));
    }
    else
    {
        cameraComponent->SetFarPlane(sSettings.getCameraFar());
    }
}
