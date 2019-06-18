#pragma once

#include "UniqueTypeId.h"

#include <string>
#include <vector>
#include <map>

class CameraComponent;
class Component;
class Collision;
class ColliderComponent;
class Material;
class MeshRendererComponent;
class Scene;

class GameObject
{
    friend class Scene;

private:
    GameObject(std::string const& name, Scene* scene);
    ~GameObject();

    void DestroyChildren();

    void RemoveComponentFromComponentWaitingList(Component* component);
    void RemoveComponentFromComponentWaitingMap(Component* component);
    void RemoveComponentFromComponentMap(Component* component);

public:
    void Update();
    void Render(CameraComponent* camera);
    void Render(CameraComponent* camera, Material* material); // Primary used to render shadow maps
    void RenderViewFrustumCullingEnabled(CameraComponent* camera);

    void CollisionEnter(Collision const& collision);
    void CollisionExit(Collision const& collision);

    void TriggerEnter(ColliderComponent* collider);
    void TriggerExit(ColliderComponent* collider);

    void SetId(std::string const& id);

    template<class T> T* AttachComponent()
    {
        T* component = new T(this);

        componentWaitingMap[component->typeId = UniqueTypeId<T>()].push_back(component);
        componentWaitingList.push_back(component);

        return component;
    }

    template<class T> T* GetComponent()
    {
        ComponentMap::const_iterator itr = componentMap.find(UniqueTypeId<T>());
        if (itr != componentMap.end() && itr->second.size())
        {
            // Return first component of this type
            return (T*)itr->second[0];
        }

        itr = componentWaitingMap.find(UniqueTypeId<T>());
        if (itr != componentWaitingMap.end() && itr->second.size())
        {
            // Return first component of this type
            return (T*)itr->second[0];
        }

        return nullptr;
    }

    template<class T> void GetComponents(std::vector<T*>& components)
    {
        ComponentMap::const_iterator itr = componentMap.find(UniqueTypeId<T>());

        if (itr != componentMap.end())
        {
            for (ComponentList::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            {
                components.push_back((T*)(*itr2));
            }
        }

        itr = componentWaitingMap.find(UniqueTypeId<T>());

        if (itr != componentWaitingMap.end())
        {
            for (ComponentList::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            {
                components.push_back((T*)(*itr2));
            }
        }
    }

    void InsertMeshRendererComponent(MeshRendererComponent* meshRendererComponent);
    void RemoveMeshRendererComponent(MeshRendererComponent* meshRendererComponent);

    std::string const& GetName() const { return name; }
    Scene* GetScene() const { return scene; }

    void Destroy() { destroyed = true; }
    bool IsDestroyed() const { return destroyed; }

private:
    typedef std::vector<Component*> ComponentList;
    typedef std::map<uint32_t /* Type Id */, ComponentList> ComponentMap;
    typedef std::vector<MeshRendererComponent*> MeshRendererComponentList;

private:
    std::string id;
    std::string name;
    ComponentMap componentWaitingMap;
    ComponentList componentWaitingList;
    ComponentMap componentMap;
    ComponentList componentList;
    MeshRendererComponentList meshRenderers;
    Scene* scene;
    bool destroyed;
};
