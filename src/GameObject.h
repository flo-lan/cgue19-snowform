#pragma once

#include "UniqueTypeId.h"

#include <string>
#include <vector>
#include <map>

class Component;
class Scene;

class GameObject
{
    friend class Scene;

private:
    GameObject(std::string const& name, Scene* scene);
    ~GameObject();

public:
    void Update();
    void Render();

    template<class T> T* AttachComponent()
    {
        T* component = new T(this);

        componentQueue[UniqueTypeId<T>()].push_back(component);

        return component;
    }

    template<class T> T* GetComponent()
    {
        ComponentMap::const_iterator itr = components.find(UniqueTypeId<T>());
        if (itr != components.end() && itr->second.size())
        {
            // Return first component of this type
            return (T*)itr->second[0];
        }

        itr = componentQueue.find(UniqueTypeId<T>());
        if (itr != componentQueue.end() && itr->second.size())
        {
            // Return first component of this type
            return (T*)itr->second[0];
        }

        return nullptr;
    }

    std::string const& GetName() const { return name; }
    Scene* GetScene() const { return scene; }

    void Destroy() { destroyed = true; }
    bool IsDestroyed() const { return destroyed; }

private:
    typedef std::map<uint32_t /* Type Id */, std::vector<Component*>> ComponentMap;

private:
    std::string name;
    ComponentMap componentQueue;
    ComponentMap components;
    Scene* scene;
    bool destroyed;
};
