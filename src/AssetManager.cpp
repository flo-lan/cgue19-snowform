#include "AssetManager.h"
#include "Utils.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "SimpleMaterial.h"
#include "StandardMaterial.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "Font.h"
#include "Cubemap.h"
#include <fstream>

void AssetManager::Unload()
{
    DeleteShaderPrograms();
    DeleteShaders();
    DeleteMaterials();
    DeleteCubemaps();
    DeleteTextures();
    DeleteMeshes();
    DeleteFonts();
}

ShaderProgram* AssetManager::GetShaderProgram(std::string const& name)
{
    ShaderProgramMap::const_iterator itr = shaderPrograms.find(name);
    return itr != shaderPrograms.end() ? itr->second : nullptr;
}

Texture2D* AssetManager::GetTexture2D(std::string const& name)
{
    TextureMap::const_iterator itr = textures.find(name);
    return itr != textures.end() ? itr->second : nullptr;
}

Cubemap* AssetManager::GetCubemap(std::string const& name)
{
    CubemapMap::const_iterator itr = cubemaps.find(name);
    return itr != cubemaps.end() ? itr->second : nullptr;
}

Mesh* AssetManager::GetMesh(std::string const& name)
{
    MeshMap::const_iterator itr = meshes.find(name);
    return itr != meshes.end() ? itr->second : nullptr;
}

Font* AssetManager::GetFont(std::string const& name)
{
    FontMap::const_iterator itr = fonts.find(name);
    return itr != fonts.end() ? itr->second : nullptr;
}

Material* AssetManager::GetMaterial(std::string const& name)
{
    for (MaterialTypeMap::const_iterator itr = materials.begin(); itr != materials.end(); itr++)
    {
        MaterialMap::const_iterator itr2 = itr->second.find(name);

        if (itr2 == itr->second.end())
        {
            continue;
        }

        return itr2->second;
    }

    return nullptr;
}

Shader* AssetManager::CreateShader(std::string const& name, GLenum type)
{
    if (shaders.find(name) != shaders.end())
    {
        fprintf(stderr, "Could not create shader '%s', because a shader with the same name is already loaded!\n", name.c_str());
        return nullptr;
    }

    return shaders[name] = new Shader(name, type);
}

void AssetManager::DeleteShaders()
{
    for (ShaderMap::const_iterator itr = shaders.begin(); itr != shaders.end(); ++itr)
    {
        delete itr->second;
    }

    shaders.clear();
}

ShaderProgram* AssetManager::CreateShaderProgram(std::string const& name)
{
    if (shaderPrograms.find(name) != shaderPrograms.end())
    {
        fprintf(stderr, "Could not create shader program '%s', because a shader program with the same name is already loaded!\n", name.c_str());
        return nullptr;
    }

    return shaderPrograms[name] = new ShaderProgram(name);
}

void AssetManager::DeleteShaderPrograms()
{
    for (ShaderProgramMap::const_iterator itr = shaderPrograms.begin(); itr != shaderPrograms.end(); ++itr)
    {
        delete itr->second;
    }

    shaderPrograms.clear();
}

Texture2D* AssetManager::CreateTexture(std::string const& name)
{
    if (textures.find(name) != textures.end())
    {
        fprintf(stderr, "Could not create texture '%s', because a texture with the same name is already loaded!\n", name.c_str());
        return nullptr;
    }

    return textures[name] = new Texture2D(name);
}

Cubemap* AssetManager::CreateCubemap(std::string const& name)
{
    if (cubemaps.find(name) != cubemaps.end())
    {
        fprintf(stderr, "Could not create cubemap '%s', because a cubemap with the same name is already loaded!\n", name.c_str());
        return nullptr;
    }

    return cubemaps[name] = new Cubemap(name);
}

void AssetManager::DeleteTextures()
{
    for (TextureMap::const_iterator itr = textures.begin(); itr != textures.end(); ++itr)
    {
        delete itr->second;
    }

    textures.clear();
}

void AssetManager::DeleteCubemaps()
{
    for (CubemapMap::const_iterator itr = cubemaps.begin(); itr != cubemaps.end(); ++itr)
    {
        delete itr->second;
    }

    cubemaps.clear();
}

void AssetManager::DeleteMaterials()
{
    for (MaterialTypeMap::const_iterator itr = materials.begin(); itr != materials.end(); ++itr)
    {
        for (MaterialMap::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            delete itr2->second;
        }
    }

    materials.clear();
}

Mesh* AssetManager::CreateQuadMesh(std::string const& name, float width, float height)
{
    return meshes[name] = Mesh::CreateQuad(name, width, height);
}

Mesh* AssetManager::CreateCubeMesh(std::string const& name, float width, float height, float depth)
{
    return meshes[name] = Mesh::CreateCube(name, width, height, depth);
}

Mesh* AssetManager::CreateCylinderMesh(std::string const& name, uint32_t segments, float radius, float height)
{
    return meshes[name] = Mesh::CreateCylinder(name, segments, radius, height);
}

Mesh* AssetManager::CreateSphereMesh(std::string const& name, uint32_t segmentsLongitude, uint32_t segmentsLatitude, float radius)
{
    return meshes[name] = Mesh::CreateSphere(name, segmentsLongitude, segmentsLatitude, radius);
}

Mesh* AssetManager::CreateTorusMesh(std::string const& name, uint32_t tubeSegments, uint32_t crossSectionSegments, float r1, float r2)
{
    return meshes[name] = Mesh::CreateTorus(name, tubeSegments, crossSectionSegments, r1, r2);
}

Mesh* AssetManager::CreateMeshFromFile(std::string const& name, std::string const& file)
{
    Mesh* mesh = Mesh::CreateFromFile(name, file);

    if (mesh)
    {
        meshes[name] = mesh;
    }

    return mesh;
}

Font* AssetManager::CreateFontFromFile(std::string const& name, std::string const& file, Texture2D* atlas)
{
    Font* font = new Font(name, atlas);

    if (!font->LoadFromFile(file))
    {
        delete font;

        return nullptr;
    }

    return fonts[name] = font;
}

void AssetManager::DeleteMeshes()
{
    for (MeshMap::const_iterator itr = meshes.begin(); itr != meshes.end(); ++itr)
    {
        delete itr->second;
    }

    meshes.clear();
}

void AssetManager::DeleteFonts()
{
    for (FontMap::const_iterator itr = fonts.begin(); itr != fonts.end(); ++itr)
    {
        delete itr->second;
    }

    fonts.clear();
}
