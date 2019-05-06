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
    SceneList loadedScenesCopy(loadedScenes);

    for (Scene* scene : loadedScenesCopy)
    {
        scene->Update();
    }
}

void SceneManager::Render()
{
    SceneList loadedScenesCopy(loadedScenes);

    for (Scene* scene : loadedScenesCopy)
    {
        scene->OnPreRender();
        scene->Render();
        scene->OnPostRender();
    }
}

void SceneManager::Unload()
{
    for (Scene* scene : loadedScenes)
    {
        delete scene;
    }

    loadedScenesByTypeId.clear();
    loadedScenes.clear();
}
