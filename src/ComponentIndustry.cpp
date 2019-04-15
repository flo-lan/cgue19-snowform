#include "ComponentIndustry.h"
#include "TransformComponentFactory.h"
#include "MeshRendererComponentFactory.h"
#include "MeshColliderComponentFactory.h"
#include "RigidDynamicComponentFactory.h"
#include "RigidStaticComponentFactory.h"

ComponentIndustry::ComponentIndustry()
{
    static TransformComponentFactory transformComponentFactory;
    static MeshRendererComponentFactory meshRendererComponentFactory;
    static MeshColliderComponentFactory meshColliderComponentFactory;
    static RigidDynamicComponentFactory rigidDynamicComponentFactory;
    static RigidStaticComponentFactory rigidStaticComponentFactory;

    factories["TransformComponent"] = &transformComponentFactory;
    factories["MeshRendererComponent"] = &meshRendererComponentFactory;
    factories["MeshColliderComponent"] = &meshColliderComponentFactory;
    factories["RigidDynamicComponent"] = &rigidDynamicComponentFactory;
    factories["RigidStaticComponent"] = &rigidStaticComponentFactory;
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
