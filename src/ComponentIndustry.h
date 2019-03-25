#pragma once

#include <string>
#include <unordered_map>

class ComponentFactory;

class ComponentIndustry
{
public:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    static ComponentIndustry& getInstance()
    {
        static ComponentIndustry instance;
        return instance;
    }

private:
    ComponentIndustry();

    typedef std::unordered_map<std::string /* Component Name */, ComponentFactory*> ComponentFactoryMap;

public:
    // We want to make sure these two methods are unacceptable,
    // otherwise we may accidentally get copies of our singleton
    ComponentIndustry(ComponentIndustry const&) = delete;
    void operator=(ComponentIndustry const&) = delete;

    void AddFactory(std::string const& componentName, ComponentFactory* factory);
    ComponentFactory* GetFactory(std::string const& componentName);

private:
    ComponentFactoryMap factories;
};

#define sComponentIndustry ComponentIndustry::getInstance()
