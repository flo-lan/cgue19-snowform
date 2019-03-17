#pragma once

#include "UniqueTypeId.h"

#include <unordered_map>
#include <vector>
#include <assert.h>

class Scene;

class SceneManager
{
private:
    typedef std::unordered_map<uint32_t /* Type Id */, Scene*> SceneMap;
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
        loadedScenes[UniqueTypeId<T>()] = scene;

        // Load() must be called here - do not move
        // it to the Scene constructor, because C++
        // does not support calling virtual methods
        // inside the constructor
        scene->Load();

        return scene;
    }

    template<class T> T* GetScene()
    {
        SceneMap::const_iterator itr = loadedScenes.find(UniqueTypeId<T>());

        if (itr == loadedScenes.end())
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

    SceneMap loadedScenes;
};

#define sSceneManager SceneManager::getInstance()
