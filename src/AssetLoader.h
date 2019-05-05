#pragma once

#include <string>

class Texture2D;

class AssetLoader
{
public:
    AssetLoader();
    virtual ~AssetLoader();

    Texture2D* LoadTextureFromFile(std::string const& name, std::string const& file);

    virtual bool LoadAssets() = 0;
    virtual void UnloadAssets() = 0;
};
