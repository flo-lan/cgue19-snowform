#pragma once

#include "UniqueTypeId.h"

#include <GL\glew.h>

#include <string>
#include <unordered_map>

class Shader;
class ShaderProgram;
class Material;
class Mesh;
class Texture2D;

class AssetManager
{
public:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    static AssetManager& getInstance()
    {
        static AssetManager instance;
        return instance;
    }

private:
    AssetManager() {}

    typedef std::unordered_map<std::string /* Shader Name */, Shader*> ShaderMap;
    typedef std::unordered_map<std::string /* Shader Program Name */, ShaderProgram*> ShaderProgramMap;
    typedef std::unordered_map<std::string /* Texture Name */, Texture2D*> TextureMap;
    typedef std::unordered_map<std::string /* Material Name */, Material*> MaterialMap;
    typedef std::unordered_map<uint32_t /* Type Id */, MaterialMap> MaterialTypeMap;
    typedef std::unordered_map<std::string /* Mesh Name */, Mesh*> MeshMap;

public:
    // We want to make sure these two methods are unacceptable,
    // otherwise we may accidentally get copies of our singleton
    AssetManager(AssetManager const&) = delete;
    void operator=(AssetManager const&) = delete;

    bool Load();
    void Unload();

    ShaderProgram* GetShaderProgram(std::string const& name);
    Texture2D* GetTexture2D(std::string const& name);
    Mesh* GetMesh(std::string const& name);

    template<class T> T* GetMaterial(std::string const& name)
    {
        MaterialTypeMap::const_iterator itr = materials.find(UniqueTypeId<T>());
        if (itr == materials.end())
        {
            return nullptr;
        }

        MaterialMap::const_iterator itr2 = itr->second.find(name);
        if (itr2 == itr->second.end())
        {
            return nullptr;
        }

        return (T*)itr2->second;
    }

private:
    Shader* CreateShader(std::string const& name, GLenum type);
    void DeleteShaders();

    ShaderProgram* CreateShaderProgram(std::string const& name);
    void DeleteShaderPrograms();

    Texture2D* CreateTexture(std::string const& name);
    void DeleteTextures();

    template<class T> T* CreateMaterial(std::string const& name, ShaderProgram* shaderProgram)
    {
        T* material = new T(name, shaderProgram);

        materials[UniqueTypeId<T>()][name] = material;

        return material;
    }
    void DeleteMaterials();

    Mesh* CreateCubeMesh(std::string const& name, float width, float height, float depth);
    Mesh* CreateCylinderMesh(std::string const& name, uint32_t segments, float radius, float height);
    Mesh* CreateSphereMesh(std::string const& name, uint32_t segmentsLongitude, uint32_t segmentsLatitude, float radius);
    Mesh* CreateTorusMesh(std::string const& name, uint32_t tubeSegments, uint32_t crossSectionSegments, float r1, float r2);
    void DeleteMeshes();

    ShaderMap shaders;
    ShaderProgramMap shaderPrograms;
    TextureMap textures;
    MaterialTypeMap materials;
    MeshMap meshes;
};

#define sAssetManager AssetManager::getInstance()
