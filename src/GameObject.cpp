#include "GameObject.h"
#include "CameraComponent.h"
#include "Component.h"
#include "Collision.h"
#include "ColliderComponent.h"
#include "Material.h"
#include "MeshRendererComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "TransformGraphTraverser.h"
#include "Mesh.h"
#include <algorithm>
#include <stack>
#include <queue>

GameObject::GameObject(std::string const& _name, Scene* _scene) :
    id(""),
    name(_name),
    scene(_scene),
    destroyed(false)
{
    fprintf(stdout, "Created game object '%s' in scene '%s'!\n", name.c_str(), scene->GetName().c_str());
}

GameObject::~GameObject()
{
    destroyed = true;

    DestroyChildren();

    for (ComponentMap::const_iterator itr = componentWaitingMap.begin(); itr != componentWaitingMap.end(); ++itr)
    {
        for (ComponentList::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            delete *itr2;
        }
    }

    for (ComponentMap::const_iterator itr = componentMap.begin(); itr != componentMap.end(); ++itr)
    {
        for (ComponentList::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            delete *itr2;
        }
    }

    // Component list contains the same elements as component map
    componentList.clear();

    if (id != "")
    {
        scene->RemoveGameObjectIdMapping(id);
    }

    fprintf(stdout, "Deleted game object '%s' from scene '%s'!\n", name.c_str(), scene->GetName().c_str());
}

void GameObject::DestroyChildren()
{
    TransformComponent* transform = GetComponent<TransformComponent>();

    if (transform == nullptr)
    {
        return;
    }

    static struct DeleteTransformGraphTraverser : public TransformGraphTraverser
    {
        virtual void Visit(TransformComponent* transform)
        {
            transform->GetOwner()->Destroy();
        }
    } t;

    transform->TraverseTransformGraphDFI(t, false);
}

void GameObject::RemoveComponentFromComponentWaitingList(Component* component)
{
    ComponentList::iterator itr = std::find(componentWaitingList.begin(), componentWaitingList.end(), component);

    if (itr != componentWaitingList.end())
    {
        componentWaitingList.erase(itr);
    }
}

void GameObject::RemoveComponentFromComponentWaitingMap(Component* component)
{
    if (!component)
    {
        return;
    }

    ComponentMap::iterator itr = componentWaitingMap.find(component->typeId);

    if (itr == componentWaitingMap.end())
    {
        return;
    }

    ComponentList::iterator itr2 = std::find(itr->second.begin(), itr->second.end(), component);

    if (itr2 != itr->second.end())
    {
        itr->second.erase(itr2);
    }
}

void GameObject::RemoveComponentFromComponentMap(Component* component)
{
    if (!component)
    {
        return;
    }

    ComponentMap::iterator itr = componentMap.find(component->typeId);

    if (itr == componentMap.end())
    {
        return;
    }

    ComponentList::iterator itr2 = std::find(itr->second.begin(), itr->second.end(), component);

    if (itr2 != itr->second.end())
    {
        itr->second.erase(itr2);
    }
}

void GameObject::Update()
{
    if (componentWaitingList.size())
    {
        // Create a copy of the component waiting list, because event calls could attach more components and modify the list
        ComponentList componentWaitingListCopy(componentWaitingList);

        // Move components from the component waiting list to the active components collection
        for (ComponentList::const_iterator itr = componentWaitingListCopy.begin(); itr != componentWaitingListCopy.end(); ++itr)
        {
            Component* component = *itr;

            RemoveComponentFromComponentWaitingMap(component);
            RemoveComponentFromComponentWaitingList(component);

            if (component->IsDestroyed())
            {
                for (ComponentList::const_iterator itr2 = componentList.begin(); itr2 != componentList.end(); ++itr2)
                {
                    (*itr2)->OnRemoveComponent(component);
                }

                delete component;
            }
            else
            {
                componentMap[component->typeId].push_back(component);
                component->OnStart();

                for (ComponentList::const_iterator itr2 = componentList.begin(); itr2 != componentList.end(); ++itr2)
                {
                    (*itr2)->OnAttachComponent(component);
                }

                componentList.push_back(component);
            }
        }
    }

    // Call Update()
    for (ComponentList::const_iterator itr = componentList.begin(); itr != componentList.end();)
    {
        Component* component = *itr;

        if (component->IsDestroyed())
        {
            RemoveComponentFromComponentMap(component);

            itr = componentList.erase(itr);

            for (ComponentList::const_iterator itr3 = componentList.begin(); itr3 != componentList.end(); ++itr3)
            {
                (*itr3)->OnRemoveComponent(component);
            }

            delete component;
        }
        else
        {
            component->Update();

            ++itr;
        }
    }

    // Call LateUpdate()
    for (ComponentList::const_iterator itr = componentList.begin(); itr != componentList.end();)
    {
        Component* component = *itr;

        if (component->IsDestroyed())
        {
            RemoveComponentFromComponentMap(component);

            itr = componentList.erase(itr);

            for (ComponentList::const_iterator itr3 = componentList.begin(); itr3 != componentList.end(); ++itr3)
            {
                (*itr3)->OnRemoveComponent(component);
            }

            delete component;
        }
        else
        {
            component->LateUpdate();

            ++itr;
        }
    }
}

void GameObject::Render(CameraComponent* camera)
{
    if (destroyed)
    {
        return;
    }

    for (MeshRendererComponentList::const_iterator itr = meshRenderers.begin(); itr != meshRenderers.end(); ++itr)
    {
        (*itr)->Render(camera);
    }
}

void GameObject::Render(CameraComponent* camera, Material* material)
{
    if (destroyed)
    {
        return;
    }

    for (MeshRendererComponentList::const_iterator itr = meshRenderers.begin(); itr != meshRenderers.end(); ++itr)
    {
        (*itr)->Render(camera, material);
    }
}

void GameObject::RenderViewFrustumCullingEnabled(CameraComponent* camera)
{
    if (destroyed)
    {
        return;
    }

    for (MeshRendererComponentList::const_iterator itr = meshRenderers.begin(); itr != meshRenderers.end(); ++itr)
    {
        (*itr)->RenderViewFrustumCullingEnabled(camera);
    }
}

void GameObject::CollisionEnter(Collision const& collision)
{
    for (ComponentList::const_iterator itr = componentList.begin(); itr != componentList.end(); ++itr)
    {
        Component* component = *itr;

        if (!component->IsDestroyed())
        {
            component->OnCollisionEnter(collision);
        }
    }
}

void GameObject::CollisionExit(Collision const& collision)
{
    for (ComponentList::const_iterator itr = componentList.begin(); itr != componentList.end(); ++itr)
    {
        Component* component = *itr;

        if (!component->IsDestroyed())
        {
            component->OnCollisionExit(collision);
        }
    }
}

void GameObject::TriggerEnter(ColliderComponent* collider)
{
    for (ComponentList::const_iterator itr = componentList.begin(); itr != componentList.end(); ++itr)
    {
        Component* component = *itr;

        if (!component->IsDestroyed())
        {
            component->OnTriggerEnter(collider);
        }
    }
}

void GameObject::TriggerExit(ColliderComponent* collider)
{
    for (ComponentList::const_iterator itr = componentList.begin(); itr != componentList.end(); ++itr)
    {
        Component* component = *itr;

        if (!component->IsDestroyed())
        {
            component->OnTriggerExit(collider);
        }
    }
}

void GameObject::SetId(std::string const& id)
{
    scene->RemoveGameObjectIdMapping(this->id);

    this->id = id;

    if (id != "")
    {
        scene->InsertGameObjectIdMapping(this, id);
    }
}

void GameObject::InsertMeshRendererComponent(MeshRendererComponent* meshRendererComponent)
{
    if (std::find(meshRenderers.begin(), meshRenderers.end(), meshRendererComponent) != meshRenderers.end())
    {
        return;
    }

    meshRenderers.push_back(meshRendererComponent);
}

void GameObject::RemoveMeshRendererComponent(MeshRendererComponent* meshRendererComponent)
{
    meshRenderers.erase(std::remove(meshRenderers.begin(), meshRenderers.end(), meshRendererComponent), meshRenderers.end());
}
