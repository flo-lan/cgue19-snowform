#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class GameObject;
class TransformComponent;
class DirectionalLightComponent;
struct SceneGraphTraverser;

class Scene
{
    friend class SceneManager;

protected:
    Scene(std::string const& name);
    virtual ~Scene();

public:
    virtual void SetOption(std::string const& key, std::string const& value) {}

    void Update();
    void PreRender();
    void Render();
    void PostRender();

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
    template<class T> T* GetComponentByGameObjectId(std::string const& id)
    {
        GameObject* gameObject = GetGameObjectById(id);

        if (!gameObject)
        {
            return nullptr;
        }

        return gameObject->GetComponent<T>();
    }

    void InsertDirectionalLightComponent(DirectionalLightComponent* light);
    void RemoveDirectionalLightComponent(DirectionalLightComponent* light);

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
    virtual void OnPostRender() {}

private:
    typedef std::vector<TransformComponent*> TransformList;
    typedef std::unordered_map<std::string /* id */, GameObject*> GameObjectByIdMap;
    typedef std::vector<DirectionalLightComponent*> DirectionalLightList;

    std::string name;
    TransformList sceneGraphRoots;
    GameObjectByIdMap gameObjectsById;
    DirectionalLightList directionalLights;
};
