#pragma once

#include "AssetLoader.h"

class GlobalAssetLoader : public AssetLoader
{
public:
    GlobalAssetLoader();
    ~GlobalAssetLoader();

    virtual bool LoadAssets();
    virtual void UnloadAssets();
};
