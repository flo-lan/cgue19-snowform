#include "Scene.h"
#include "SceneGraphTraverser.h"
#include "Screen.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "ComponentIndustry.h"
#include "ComponentFactory.h"
#include "tinyxml2.h"
#include <algorithm>
#include <stack>
#include <queue>

Scene::Scene(std::string const& _name) :
    name(_name),
    file("")
{
}

Scene::~Scene()
{
    Unload();
}

void Scene::Unload()
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

    sceneGraphRoots.clear();
    gameObjectsById.clear();
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

bool Scene::LoadFromFile(std::string file)
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

    this->file = file;

    OnLoad();

    return true;
}

void Scene::Reload()
{
    Unload();

    if (file.empty())
    {
        Load();
    }
    else
    {
        LoadFromFile(file);
    }
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

void Scene::PreRender()
{
    if (camera)
    {
        for (auto itr = directionalLights.begin(); itr != directionalLights.end(); ++itr)
        {
            DirectionalLightComponent* light = *itr;

            if (light->IsShadowEnabled())
            {
                light->RenderShadowMaps(camera);
            }
        }

        glViewport(0, 0, Screen::GetWidth(), Screen::GetHeight());
    }

    camera->ObjectsRendered = 0;
    camera->ObjectsCulled = 0;

    OnPreRender();
}

struct RenderSceneGraphViewFrustumCullingEnabledTraverser : public SceneGraphTraverser
{
    CameraComponent* Camera;

    RenderSceneGraphViewFrustumCullingEnabledTraverser(CameraComponent* camera) :
        Camera(camera)
    {}

    virtual void Visit(TransformComponent* transform)
    {
        transform->GetOwner()->RenderViewFrustumCullingEnabled(Camera);
    }
};

struct RenderSceneGraphTraverser : public SceneGraphTraverser
{
    CameraComponent* Camera;

    RenderSceneGraphTraverser(CameraComponent* camera) :
        Camera(camera)
    {}

    virtual void Visit(TransformComponent* transform)
    {
        transform->GetOwner()->Render(Camera);
    }
};

void Scene::Render()
{
    if (!camera)
    {
        return;
    }

    if (camera->IsViewFrustumCullingEnabled())
    {
        RenderSceneGraphViewFrustumCullingEnabledTraverser traverser(camera);

        TraverseSceneGraphDF(traverser);
    }
    else
    {
        RenderSceneGraphTraverser traverser(camera);

        TraverseSceneGraphDF(traverser);
    }
}

void Scene::PostRender()
{
    OnPostRender();
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

void Scene::InsertGameObjectIdMapping(GameObject* gameObject, std::string const& id)
{
    gameObjectsById[id] = gameObject;
}

void Scene::RemoveGameObjectIdMapping(std::string const& id)
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

void Scene::InsertDirectionalLightComponent(DirectionalLightComponent* light)
{
    if (std::find(directionalLights.begin(), directionalLights.end(), light) != directionalLights.end())
    {
        return;
    }

    directionalLights.push_back(light);
}

void Scene::RemoveDirectionalLightComponent(DirectionalLightComponent* light)
{
    directionalLights.erase(std::remove(directionalLights.begin(), directionalLights.end(), light), directionalLights.end());
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
