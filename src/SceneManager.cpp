#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
    SceneList loadedScenesCopy;

    for (SceneMap::const_iterator itr = loadedScenes.begin(); itr != loadedScenes.end(); ++itr)
    {
        loadedScenesCopy.push_back(itr->second);
    }

    for (Scene* scene : loadedScenesCopy)
    {
        scene->Update();
    }
}

void SceneManager::Render()
{
    for (SceneMap::const_iterator itr = loadedScenes.begin(); itr != loadedScenes.end(); ++itr)
    {
        itr->second->Render();
    }
}

void SceneManager::Unload()
{
    for (SceneMap::const_iterator itr = loadedScenes.begin(); itr != loadedScenes.end(); ++itr)
    {
        delete itr->second;
    }

    loadedScenes.clear();
}
