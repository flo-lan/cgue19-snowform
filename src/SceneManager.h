#pragma once

#include "UniqueTypeId.h"

#include <assert.h>
#include <string>
#include <unordered_map>
#include <vector>

class Scene;

class SceneManager
{
private:
    typedef std::unordered_map<uint32_t /* Type Id */, Scene*> SceneByTypeIdMap;
    typedef std::vector<Scene*> SceneList;

public:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    static SceneManager& getInstance()
    {
        static SceneManager instance;
        return instance;
    }

    // We want to make sure these two methods are unacceptable,
    // otherwise we may accidentally get copies of our singleton
    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;

    template<class T> Scene* LoadScene()
    {
        // Avoid to load the same scene twice
        assert(GetScene<T>() == nullptr);

        // Create new scene object and call default constructor
        T* scene = new T();

        // Add the new scene object to the loaded scenes map
        loadedScenesByTypeId[UniqueTypeId<T>()] = scene;

        // Add the new scene object to the ordered loaded scenes list
        loadedScenes.push_back(scene);

        // Load() must be called here - do not move
        // it to the Scene constructor, because C++
        // does not support calling virtual methods
        // inside the constructor
        scene->Load();

        return scene;
    }

    template<class T> Scene* LoadSceneFromFile(std::string const& file)
    {
        // Avoid to load the same scene twice
        assert(GetScene<T>() == nullptr);

        // Create new scene object and call default constructor
        T* scene = new T();

        // Add the new scene object to the loaded scenes map
        loadedScenesByTypeId[UniqueTypeId<T>()] = scene;

        // Load() must be called here - do not move
        // it to the Scene constructor, because C++
        // does not support calling virtual methods
        // inside the constructor
        if (!scene->LoadFromFile(file))
        {
            loadedScenesByTypeId.erase(UniqueTypeId<T>());

            delete scene;

            scene = nullptr;
        }

        // Add the new scene object to the ordered loaded scenes list
        loadedScenes.push_back(scene);

        return scene;
    }

    template<class T> T* GetScene()
    {
        SceneByTypeIdMap::const_iterator itr = loadedScenesByTypeId.find(UniqueTypeId<T>());

        if (itr == loadedScenesByTypeId.end())
        {
            return nullptr;
        }

        return (T*)itr->second;
    }

    void Update();
    void Render();

    void Unload();

private:
    SceneManager();
    ~SceneManager();

    SceneByTypeIdMap loadedScenesByTypeId;
    SceneList loadedScenes;
};

#define sSceneManager SceneManager::getInstance()
