#include "Scene.h"
#include "SceneGraphTraverser.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ComponentIndustry.h"
#include "ComponentFactory.h"
#include "tinyxml2.h"
#include <algorithm>
#include <stack>
#include <queue>

Scene::Scene(std::string const& _name) :
    name(_name)
{
}

Scene::~Scene()
{
    static struct DeleteSceneGraphTraverser : public SceneGraphTraverser
    {
        virtual void Visit(TransformComponent* transform)
        {
            GameObject* gameObject = transform->GetOwner();

            gameObject->GetScene()->DeleteGameObject(gameObject);
        }
    } t;

    TraverseSceneGraphDFI(t);
}

struct GameObjectElement
{
    GameObject* Parent;
    tinyxml2::XMLElement* Element;

    GameObjectElement(GameObject* parent, tinyxml2::XMLElement* element) :
        Parent(parent),
        Element(element)
    {}
};

bool Scene::LoadFromFile(std::string const& file)
{
    tinyxml2::XMLDocument doc;

    switch (doc.LoadFile(file.c_str()))
    {
        case tinyxml2::XML_SUCCESS:
            break;
        default:
            return false;
    }

    tinyxml2::XMLElement* rootElement = doc.RootElement();

    if (rootElement == nullptr)
    {
        return false;
    }

    std::string rootElementName = std::string(rootElement->Name());

    if (rootElementName != "Scene")
    {
        return false;
    }

    std::queue<GameObjectElement> gameObjectQueue;

    for (tinyxml2::XMLElement* childElement = rootElement->FirstChildElement("GameObject"); childElement != nullptr; childElement = childElement->NextSiblingElement("GameObject"))
    {
        gameObjectQueue.push(GameObjectElement(nullptr, childElement));
    }

    while (gameObjectQueue.size())
    {
        GameObjectElement& gameObjectElement = gameObjectQueue.front();

        gameObjectQueue.pop();

        const char* gameObjectName = gameObjectElement.Element->Attribute("name");
        GameObject* gameObject = CreateGameObject
        (
            gameObjectName ? std::string(gameObjectName) : std::string(),
            gameObjectElement.Parent ? gameObjectElement.Parent->GetComponent<TransformComponent>() : nullptr
        );

        const char* gameObjectId = gameObjectElement.Element->Attribute("id");
        if (gameObjectId)
        {
            gameObject->SetId(std::string(gameObjectId));
        }

        if (tinyxml2::XMLNode* componentsNode = gameObjectElement.Element->FirstChildElement("Components"))
        {
            for (tinyxml2::XMLElement* componentElement = componentsNode->FirstChildElement(); componentElement != nullptr; componentElement = componentElement->NextSiblingElement())
            {
                if (ComponentFactory* factory = sComponentIndustry.GetFactory(std::string(componentElement->Name())))
                {
                    factory->Build(gameObject, componentElement);
                }
                else
                {
                    fprintf(stderr, "Could not find component factory for component '%s' of game object '%s'!\n", componentElement->Name(), gameObject->GetName().c_str());
                }
            }
        }

        tinyxml2::XMLNode* childrenNode = gameObjectElement.Element->FirstChildElement("Children");

        if (!childrenNode)
        {
            continue;
        }

        for (tinyxml2::XMLElement* childElement = childrenNode->FirstChildElement("GameObject"); childElement != nullptr; childElement = childElement->NextSiblingElement("GameObject"))
        {
            gameObjectQueue.push(GameObjectElement(gameObject, childElement));
        }
    }

    OnLoad();

    return true;
}

void Scene::Update()
{
    static struct UpdateSceneGraphTraverser : public SceneGraphTraverser
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
    static struct RenderSceneGraphTraverser : public SceneGraphTraverser
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

void Scene::InsertGameObjectById(GameObject* gameObject, std::string const& id)
{
    gameObjectsById[id] = gameObject;
}

void Scene::RemoveGameObjectById(std::string const& id)
{
    gameObjectsById.erase(id);
}

GameObject* Scene::GetGameObjectById(std::string const& id)
{
    GameObjectByIdMap::const_iterator itr = gameObjectsById.find(id);

    if (itr != gameObjectsById.end())
    {
        return itr->second;
    }

    return nullptr;
}

GameObject* Scene::CreateGameObject(std::string const& name, TransformComponent* parent)
{
    GameObject* gameObject = new GameObject(name, this);

    // Every game object should have a transform component attached
    TransformComponent* transform = gameObject->AttachComponent<TransformComponent>();

    if (parent)
    {
        // Set parent
        transform->SetParent(parent, false);
    }

    return gameObject;
}

void Scene::DeleteGameObject(GameObject* gameObject)
{
    delete gameObject;
}
