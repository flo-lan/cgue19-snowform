#pragma once

#include <string>

class Texture2D;
class ShaderProgram;

class AssetLoader
{
public:
    AssetLoader();
    virtual ~AssetLoader();

    ShaderProgram* LoadShaderProgramFromFiles(std::string const& name,
        std::string const& vertexShaderName, std::string const& vertexShaderFile,
        std::string const& fragmentShaderName, std::string const& fragmentShaderFile);
    Texture2D* LoadTextureFromFile(std::string const& name, std::string const& file, bool generateMipMaps = true);

    virtual bool LoadAssets() = 0;
    virtual void UnloadAssets() = 0;
};
