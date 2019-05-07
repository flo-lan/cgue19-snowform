#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class GameObject;
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

    void Unload();

    std::string const& GetName() const { return name; }

    void InsertSceneGraphRoot(TransformComponent* transform);
    void RemoveSceneGraphRoot(TransformComponent* transform);

    // Traverse scene graph depth first
    void TraverseSceneGraphDF(SceneGraphTraverser& traverser);
    // Traverse scene graph depth first inversed
    void TraverseSceneGraphDFI(SceneGraphTraverser& traverser);

    void InsertGameObjectIdMapping(GameObject* gameObject, std::string const& id);
    void RemoveGameObjectIdMapping(std::string const& id);
    
    GameObject* GetGameObjectById(std::string const& id);

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
    virtual void OnPreRender() {}
    virtual void OnRender() {}
    virtual void OnPostRender() {}

private:
    typedef std::vector<TransformComponent*> TransformList;
    typedef std::unordered_map<std::string /* id */, GameObject*> GameObjectByIdMap;

    std::string name;
    TransformList sceneGraphRoots;
    GameObjectByIdMap gameObjectsById;
};
