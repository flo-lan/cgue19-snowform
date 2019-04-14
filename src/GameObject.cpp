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

void GameObject::Update()
{
    if (componentQueue.size())
    {
        // Create a copy of the component queue, because event calls could attach more components and modify the queue
        ComponentMap componentQueueCopy(componentQueue);

        // Move components from the component queue to the active components collection
        for (ComponentMap::const_iterator itr = componentQueueCopy.begin(); itr != componentQueueCopy.end(); ++itr)
        {
            ComponentMap::iterator itr4 = componentQueue.find(itr->first);

            for (ComponentList::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
            {
                Component* component = *itr2;

                if (itr4 != componentQueue.end())
                {
                    ComponentList::iterator itr3 = std::find(itr4->second.begin(), itr4->second.end(), component);

                    if (itr3 != itr4->second.end())
                    {
                        itr4->second.erase(itr3);

                        // Remove empty lists
                        if (itr4->second.size() == 0)
                        {
                            componentQueue.erase(itr4);
                        }
                    }
                }

                if (component->IsDestroyed())
                {
                    for (ComponentList::const_iterator itr3 = componentList.begin(); itr3 != componentList.end(); ++itr3)
                    {
                        (*itr3)->OnRemoveComponent(component);
                    }

                    delete component;

                    continue;
                }

                componentMap[itr->first].push_back(component);
                component->OnStart();

                for (ComponentList::const_iterator itr3 = componentList.begin(); itr3 != componentList.end(); ++itr3)
                {
                    (*itr3)->OnAttachComponent(component);
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
            ComponentMap::iterator itr2 = componentMap.find(component->typeId);

            if (itr2 != componentMap.end())
            {
                ComponentList::iterator itr3 = std::find(itr2->second.begin(), itr2->second.end(), component);

                if (itr3 != itr2->second.end())
                {
                    itr2->second.erase(itr3);

                    // Remove empty lists
                    if (itr2->second.size() == 0)
                    {
                        componentMap.erase(itr2);
                    }
                }
            }

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
            ComponentMap::iterator itr2 = componentMap.find(component->typeId);

            if (itr2 != componentMap.end())
            {
                ComponentList::iterator itr3 = std::find(itr2->second.begin(), itr2->second.end(), component);

                if (itr3 != itr2->second.end())
                {
                    itr2->second.erase(itr3);

                    // Remove empty lists
                    if (itr2->second.size() == 0)
                    {
                        componentMap.erase(itr2);
                    }
                }
            }

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

void GameObject::Render()
{
    for (ComponentList::const_iterator itr = componentList.begin(); itr != componentList.end(); ++itr)
    {
        (*itr)->Render();
    }
}
