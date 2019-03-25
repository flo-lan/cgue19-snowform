#include "ComponentIndustry.h"
#include "TransformComponentFactory.h"
#include "MeshRendererComponentFactory.h"

ComponentIndustry::ComponentIndustry()
{
    static TransformComponentFactory transformComponentFactory;
    static MeshRendererComponentFactory meshRendererComponentFactory;

    factories["TransformComponent"] = &transformComponentFactory;
    factories["MeshRendererComponent"] = &meshRendererComponentFactory;
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
