#include "Scene.h"
#include "SceneGraphTraverser.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <algorithm>
#include <stack>
#include <queue>

Scene::Scene(std::string const& _name) :
    name(_name)
{
}

Scene::~Scene()
{
    struct DeleteSceneGraphTraverser : public SceneGraphTraverser
    {
        virtual void Visit(TransformComponent* transform)
        {
            GameObject* gameObject = transform->GetOwner();

            gameObject->GetScene()->DeleteGameObject(gameObject);
        }
    } t;

    TraverseSceneGraphDFI(t);
}

void Scene::Update()
{
    struct UpdateSceneGraphTraverser : public SceneGraphTraverser
    {
        virtual void Visit(TransformComponent* transform)
        {
            GameObject* gameObject = transform->GetOwner();

            if (!gameObject->IsDestroyed())
            {
                gameObject->Update();
            }

            if (gameObject->IsDestroyed())
            {
                gameObject->GetScene()->DeleteGameObject(gameObject);
            }
        }
    } t;

    TraverseSceneGraphDF(t);

    OnUpdate();
}

void Scene::Render()
{
    struct RenderSceneGraphTraverser : public SceneGraphTraverser
    {
        virtual void Visit(TransformComponent* transform)
        {
            GameObject* gameObject = transform->GetOwner();

            if (!gameObject->IsDestroyed())
            {
                gameObject->Render();
            }

            if (gameObject->IsDestroyed())
            {
                gameObject->GetScene()->DeleteGameObject(gameObject);
            }
        }
    } t;

    TraverseSceneGraphDF(t);

    OnRender();
}

void Scene::InsertSceneGraphRoot(TransformComponent* transform)
{
    if (!transform)
    {
        return;
    }

    if (transform->GetParent())
    {
        return;
    }

    sceneGraphRoots.push_back(transform);
}

void Scene::RemoveSceneGraphRoot(TransformComponent* transform)
{
    if (!transform)
    {
        return;
    }

    sceneGraphRoots.erase(std::remove(sceneGraphRoots.begin(), sceneGraphRoots.end(), transform), sceneGraphRoots.end());
}

void Scene::TraverseSceneGraphDF(SceneGraphTraverser& traverser)
{
    TransformList sceneGraphRootsCopy(sceneGraphRoots);

    for (TransformList::const_iterator itr = sceneGraphRootsCopy.begin(); itr != sceneGraphRootsCopy.end(); itr++)
    {
        (*itr)->TraverseTransformGraphDF(traverser, true);
    }
}

void Scene::TraverseSceneGraphDFI(SceneGraphTraverser& traverser)
{
    TransformList sceneGraphRootsCopy(sceneGraphRoots);

    for (TransformList::const_iterator itr = sceneGraphRootsCopy.begin(); itr != sceneGraphRootsCopy.end(); itr++)
    {
        (*itr)->TraverseTransformGraphDFI(traverser, true);
    }
}

GameObject* Scene::CreateGameObject(std::string const& name, TransformComponent* parent)
{
    GameObject* gameObject = new GameObject(name, this);

    // Every game object should have a transform component attached
    TransformComponent* transform = gameObject->AttachComponent<TransformComponent>();

    if (parent)
    {
        // Set parent
        transform->SetParent(parent);
    }

    return gameObject;
}

void Scene::DeleteGameObject(GameObject* gameObject)
{
    delete gameObject;
}
