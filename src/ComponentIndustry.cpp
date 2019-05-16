#include "ComponentIndustry.h"
#include "TransformComponentFactory.h"
#include "MeshRendererComponentFactory.h"
#include "MeshColliderComponentFactory.h"
#include "RigidDynamicComponentFactory.h"
#include "RigidStaticComponentFactory.h"
#include "BoxColliderComponentFactory.h"
#include "SphereColliderComponentFactory.h"
#include "ArcBallControllerComponentFactory.h"
#include "CameraComponentFactory.h"
#include "DebugComponentFactory.h"
#include "ThirdPersonControllerComponentFactory.h"
#include "MovingPlatformComponentFactory.h"
#include "RotateComponentFactory.h"
#include "TextComponentFactory.h"
#include "CoinComponentFactory.h"
#include "FlagComponentFactory.h"
#include "SceneOptionComponentFactory.h"
#include "PlayerComponentFactory.h"

ComponentIndustry::ComponentIndustry()
{
    static TransformComponentFactory transformComponentFactory;
    static MeshRendererComponentFactory meshRendererComponentFactory;
    static MeshColliderComponentFactory meshColliderComponentFactory;
    static RigidDynamicComponentFactory rigidDynamicComponentFactory;
    static RigidStaticComponentFactory rigidStaticComponentFactory;
    static BoxColliderComponentFactory boxColliderComponentFactory;
    static SphereColliderComponentFactory sphereColliderComponentFactory;
    static ArcBallControllerComponentFactory arcBallControllerComponentFactory;
    static ThirdPersonControllerComponentFactory thirdPersonControllerComponentFactory;
    static CameraComponentFactory cameraComponentFactory;
    static DebugComponentFactory debugComponentFactory;
    static MovingPlatformComponentFactory movingPlatformComponentFactory;
    static RotateComponentFactory rotateComponentFactory;
    static TextComponentFactory textComponentFactory;
    static CoinComponentFactory coinComponentFactory;
    static FlagComponentFactory flagComponentFactory;
    static SceneOptionComponentFactory sceneOptionComponentFactory;
    static PlayerComponentFactory playerComponentFactory;

    factories["TransformComponent"] = &transformComponentFactory;
    factories["MeshRendererComponent"] = &meshRendererComponentFactory;
    factories["MeshColliderComponent"] = &meshColliderComponentFactory;
    factories["RigidDynamicComponent"] = &rigidDynamicComponentFactory;
    factories["RigidStaticComponent"] = &rigidStaticComponentFactory;
    factories["BoxColliderComponent"] = &boxColliderComponentFactory;
    factories["SphereColliderComponent"] = &sphereColliderComponentFactory;
    factories["ArcBallControllerComponent"] = &arcBallControllerComponentFactory;
    factories["ThirdPersonControllerComponent"] = &thirdPersonControllerComponentFactory;
    factories["CameraComponent"] = &cameraComponentFactory;
    factories["DebugComponent"] = &debugComponentFactory;
    factories["MovingPlatformComponent"] = &movingPlatformComponentFactory;
    factories["RotateComponent"] = &rotateComponentFactory;
    factories["TextComponent"] = &textComponentFactory;
    factories["CoinComponent"] = &coinComponentFactory;
    factories["FlagComponent"] = &flagComponentFactory;
    factories["SceneOptionComponent"] = &sceneOptionComponentFactory;
    factories["PlayerComponent"] = &playerComponentFactory;
}

void ComponentIndustry::AddFactory(std::string const& componentName, ComponentFactory* factory)
{
    factories[componentName] = factory;
}

ComponentFactory* ComponentIndustry::GetFactory(std::string const& componentName)
{
    ComponentFactoryMap::const_iterator itr = factories.find(componentName);

    if (itr == factories.end())
    {
        return nullptr;
    }

    return itr->second;
}
