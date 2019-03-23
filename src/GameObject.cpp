#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "TransformGraphTraverser.h"
#include <algorithm>
#include <stack>
#include <queue>

GameObject::GameObject(std::string const& _name, Scene* _scene) :
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

    for (ComponentMap::const_iterator itr = componentQueue.begin(); itr != componentQueue.end(); ++itr)
    {
        for (std::vector<Component*>::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            delete *itr2;
        }
    }

    for (ComponentMap::const_iterator itr = components.begin(); itr != components.end(); ++itr)
    {
        for (std::vector<Component*>::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            delete *itr2;
        }
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

    struct DeleteTransformGraphTraverser : public TransformGraphTraverser
    {
        virtual void Visit(TransformComponent* transform)
        {
            transform->GetOwner()->Destroy();
        }
    } t;

    transform->TraverseTransformGraphDFI(t, false);
}

void GameObject::Update()
{
    if (componentQueue.size())
    {
        ComponentMap componentQueueCopy(componentQueue);

        // Move components from the component queue to the active components collection
        for (ComponentMap::const_iterator itr = componentQueue.begin(); itr != componentQueue.end(); ++itr)
        {
            for (std::vector<Component*>::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            {
                components[itr->first].push_back(*itr2);
            }
        }

        componentQueue.clear();

        // Call OnStart() events - must be called after all elements from the component queue are moved,
        // because some OnStart() method calls could attach more components and modify the queue
        for (ComponentMap::const_iterator itr = componentQueueCopy.begin(); itr != componentQueueCopy.end(); ++itr)
        {
            for (std::vector<Component*>::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            {
                Component* component = *itr2;

                if (!component->IsDestroyed())
                {
                    component->OnStart();
                }
            }
        }
    }

    // Call Update()
    for (ComponentMap::iterator itr = components.begin(); itr != components.end();)
    {
        for (std::vector<Component*>::iterator itr2 = itr->second.begin(); itr2 != itr->second.end();)
        {
            Component* component = *itr2;

            if (component->IsDestroyed())
            {
                delete component;
                itr2 = itr->second.erase(itr2);
            }
            else
            {
                component->Update();
                ++itr2;
            }
        }

        // Remove component types with empty lists
        if (itr->second.size() == 0)
        {
            itr = components.erase(itr);
        }
        else
        {
            ++itr;
        }
    }

    // Call LateUpdate()
    for (ComponentMap::iterator itr = components.begin(); itr != components.end();)
    {
        for (std::vector<Component*>::iterator itr2 = itr->second.begin(); itr2 != itr->second.end();)
        {
            Component* component = *itr2;

            if (component->IsDestroyed())
            {
                delete component;
                itr2 = itr->second.erase(itr2);
            }
            else
            {
                component->LateUpdate();
                ++itr2;
            }
        }

        // Remove component types with empty lists
        if (itr->second.size() == 0)
        {
            itr = components.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
}

void GameObject::Render()
{
    for (ComponentMap::const_iterator itr = components.begin(); itr != components.end(); ++itr)
    {
        for (std::vector<Component*>::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            (*itr2)->Render();
        }
    }
}
