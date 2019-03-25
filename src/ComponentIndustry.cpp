#include "ComponentIndustry.h"
#include "TransformComponentFactory.h"

ComponentIndustry::ComponentIndustry()
{
    static TransformComponentFactory transformComponentFactory;

    factories["TransformComponent"] = &transformComponentFactory;
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
