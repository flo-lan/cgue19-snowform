#include "AssetLoader.h"
#include "AssetManager.h"
#include "Texture2D.h"

AssetLoader::AssetLoader()
{
}

AssetLoader::~AssetLoader()
{
}

Texture2D* AssetLoader::LoadTextureFromFile(std::string const& name, std::string const& file)
{
    Texture2D* texture = sAssetManager.CreateTexture(name);

    if (!texture)
    {
        return nullptr;
    }

    if (!texture->LoadFromFile(file))
    {
        return nullptr;
    }

    return texture;
}
