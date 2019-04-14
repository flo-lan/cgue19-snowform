#pragma once

#include <string>
#include <vector>

class GameObject;
class PhysicsScene;
class TransformComponent;
struct SceneGraphTraverser;

class Scene
{
    friend class SceneManager;

protected:
    Scene(std::string const& name);
    virtual ~Scene();

public:
    void Update();
    void Render();

    std::string const& GetName() const { return name; }
    PhysicsScene* GetPhysicsScene() const { return physicsScene; }

    void InsertSceneGraphRoot(TransformComponent* transform);
    void RemoveSceneGraphRoot(TransformComponent* transform);

    // Traverse scene graph depth first
    void TraverseSceneGraphDF(SceneGraphTraverser& traverser);
    // Traverse scene graph depth first inversed
    void TraverseSceneGraphDFI(SceneGraphTraverser& traverser);

    // Call this for creating a new game object in this scene
    GameObject* CreateGameObject(std::string const& name, TransformComponent* parent = nullptr);

private:
    // Outside usage forbidden - if you want to delete a game object call GameObject::Destroy()
    void DeleteGameObject(GameObject* gameObject);

    void Load() { OnLoad(); }
    bool LoadFromFile(std::string const& file);

protected:
    virtual void OnLoad() {}
    virtual void OnUpdate() {}
    virtual void OnRender() {}

private:
    typedef std::vector<TransformComponent*> TransformList;

    std::string name;
    TransformList sceneGraphRoots;
    PhysicsScene* physicsScene;
};
