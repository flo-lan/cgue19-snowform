#pragma once

#include <string>
#include <vector>

class GameObject;

class Scene
{
    friend class SceneManager;

private:
    typedef std::vector<GameObject*> GameObjectList;

protected:
    Scene(std::string const& name);
    virtual ~Scene();

public:
    void Update();
    void Render();

    std::string const& GetName() const { return name; }

    // Call this for creating a new game object in this scene
    GameObject* CreateGameObject(std::string const& name);

private:
    // Only called in Scene::Update() if GameObject::IsDestroyed() returns true
    bool DeleteGameObject(GameObject* gameObject); // If you want to delete a game object call GameObject::Destroy()

    void Load() { OnLoad(); }

protected:
    virtual void OnLoad() {}
    virtual void OnUpdate() {}
    virtual void OnRender() {}

private:
    std::string name;
    GameObjectList gameObjects;
};
