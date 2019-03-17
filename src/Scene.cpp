#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

Scene::Scene(std::string const& _name) :
    name(_name)
{
}

Scene::~Scene()
{
    for (GameObject* gameObject : gameObjects)
    {
        delete gameObject;
    }

    gameObjects.clear();
}

void Scene::Update()
{
    GameObjectList gameObjectsCopy(gameObjects);

    for (GameObject* gameObject : gameObjectsCopy)
    {
        if (gameObject->IsDestroyed())
        {
            DeleteGameObject(gameObject);
        }
        else
        {
            gameObject->Update();
        }
    }

    OnUpdate();
}

void Scene::Render()
{
    for (GameObject* gameObject : gameObjects)
    {
        gameObject->Render();
    }

    OnRender();
}

GameObject* Scene::CreateGameObject(std::string const& name)
{
    GameObject* gameObject = new GameObject(name, this);

    gameObjects.push_back(gameObject);

    return gameObject;
}

bool Scene::DeleteGameObject(GameObject* gameObject)
{
    GameObjectList::const_iterator itr = std::remove(gameObjects.begin(), gameObjects.end(), gameObject);

    if (itr == gameObjects.end())
    {
        return false;
    }

    gameObjects.erase(itr, gameObjects.end());

    delete gameObject;

    return true;
}
