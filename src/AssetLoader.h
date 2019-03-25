#pragma once

class AssetLoader
{
public:
    AssetLoader();
    virtual ~AssetLoader();

    virtual bool LoadAssets() = 0;
    virtual void UnloadAssets() = 0;
};
