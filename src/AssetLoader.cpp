#include "AssetLoader.h"
#include "AssetManager.h"
#include "Texture2D.h"
#include "ShaderProgram.h"
#include "Shader.h"

AssetLoader::AssetLoader()
{
}

AssetLoader::~AssetLoader()
{
}

ShaderProgram* AssetLoader::LoadShaderProgramFromFiles(std::string const& name,
    std::string const& vertexShaderName, std::string const& vertexShaderFile,
    std::string const& fragmentShaderName, std::string const& fragmentShaderFile)
{
    Shader* vertexShader = sAssetManager.CreateShader(vertexShaderName, GL_VERTEX_SHADER);
    Shader* fragmentShader = sAssetManager.CreateShader(fragmentShaderName, GL_FRAGMENT_SHADER);

    if (!vertexShader) { return nullptr; }
    if (!fragmentShader) { return nullptr; }

    if (!vertexShader->LoadSourceFromFile(vertexShaderFile)) { return nullptr; }
    if (!fragmentShader->LoadSourceFromFile(fragmentShaderFile)) { return nullptr; }

    if (!vertexShader->Compile()) { return nullptr; }
    if (!fragmentShader->Compile()) { return nullptr; }

    ShaderProgram* shaderProgram = sAssetManager.CreateShaderProgram(name);

    if (!shaderProgram) { return nullptr; }

    if (!shaderProgram->AttachShader(vertexShader)) { return nullptr; }
    if (!shaderProgram->AttachShader(fragmentShader)) { return nullptr; }

    if (!shaderProgram->Link())
    {
        return nullptr;
    }

    return shaderProgram;
}

ShaderProgram* AssetLoader::LoadComputeShaderProgramFromFile(std::string const& name, std::string const& computeShaderName, std::string const& computeShaderFile)
{
    Shader* computeShader = sAssetManager.CreateShader(computeShaderName, GL_COMPUTE_SHADER);

    if (!computeShader)
    {
        return nullptr;
    }

    if (!computeShader->LoadSourceFromFile(computeShaderFile))
    {
        return nullptr;
    }

    if (!computeShader->Compile())
    {
        return nullptr;
    }

    ShaderProgram* computeShaderProgram = sAssetManager.CreateShaderProgram(name);

    if (!computeShaderProgram)
    {
        return nullptr;
    }

    if (!computeShaderProgram->AttachShader(computeShader))
    {
        return nullptr;
    }

    if (!computeShaderProgram->Link())
    {
        return nullptr;
    }

    return computeShaderProgram;
}

Texture2D* AssetLoader::LoadTextureFromFile(std::string const& name, std::string const& file, bool generateMipMaps)
{
    Texture2D* texture = sAssetManager.CreateTexture(name);

    if (!texture)
    {
        return nullptr;
    }

    if (!texture->LoadFromFile(file, generateMipMaps))
    {
        return nullptr;
    }

    return texture;
}
