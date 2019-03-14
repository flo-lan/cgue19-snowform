#include "AssetManager.h"
#include "Utils.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "SimpleMaterial.h"
#include "PhongGouraudMaterial.h"
#include "Mesh.h"
#include "Texture2D.h"

#include <fstream>

bool AssetManager::Load()
{
    Shader* simpleTeapotVertexShader = CreateShader("simple_teapot_vertex_shader", GL_VERTEX_SHADER);
    Shader* simpleTeapotFragmentShader = CreateShader("simple_teapot_fragment_shader", GL_FRAGMENT_SHADER);
    Shader* phongVertexShader = CreateShader("phong_vertex_shader", GL_VERTEX_SHADER);
    Shader* phongFragmentShader = CreateShader("phong_fragment_shader", GL_FRAGMENT_SHADER);
    Shader* gouraudVertexShader = CreateShader("gouraud_vertex_shader", GL_VERTEX_SHADER);
    Shader* gouraudFragmentShader = CreateShader("gouraud_fragment_shader", GL_FRAGMENT_SHADER);

    if (!simpleTeapotVertexShader) { return false; }
    if (!simpleTeapotFragmentShader) { return false; }
    if (!phongVertexShader) { return false; }
    if (!phongFragmentShader) { return false; }
    if (!gouraudVertexShader) { return false; }
    if (!gouraudFragmentShader) { return false; }

    if (!simpleTeapotVertexShader->LoadSourceFromFile("assets/shaders/simple_teapot_vertex.glsl")) { return false; }
    if (!simpleTeapotFragmentShader->LoadSourceFromFile("assets/shaders/simple_teapot_fragment.glsl")) { return false; }
    if (!phongVertexShader->LoadSourceFromFile("assets/shaders/phong_vertex.glsl")) { return false; }
    if (!phongFragmentShader->LoadSourceFromFile("assets/shaders/phong_fragment.glsl")) { return false; }
    if (!gouraudVertexShader->LoadSourceFromFile("assets/shaders/gouraud_vertex.glsl")) { return false; }
    if (!gouraudFragmentShader->LoadSourceFromFile("assets/shaders/gouraud_fragment.glsl")) { return false; }

    if (!simpleTeapotVertexShader->Compile()) { return false; }
    if (!simpleTeapotFragmentShader->Compile()) { return false; }
    if (!phongVertexShader->Compile()) { return false; }
    if (!phongFragmentShader->Compile()) { return false; }
    if (!gouraudVertexShader->Compile()) { return false; }
    if (!gouraudFragmentShader->Compile()) { return false; }

    ShaderProgram* simpleTeapotShaderProgram1 = CreateShaderProgram("simple_teapot_shader_program1");
    ShaderProgram* phongShaderProgram = CreateShaderProgram("phong_shader_program");
    ShaderProgram* gouraudShaderProgram = CreateShaderProgram("gouraud_shader_program");

    if (!simpleTeapotShaderProgram1) { return false; }
    if (!phongShaderProgram) { return false; }
    if (!gouraudShaderProgram) { return false; }

    if (!simpleTeapotShaderProgram1->AttachShader(simpleTeapotVertexShader)) { return false; }
    if (!simpleTeapotShaderProgram1->AttachShader(simpleTeapotFragmentShader)) { return false; }
    if (!phongShaderProgram->AttachShader(phongVertexShader)) { return false; }
    if (!phongShaderProgram->AttachShader(phongFragmentShader)) { return false; }
    if (!gouraudShaderProgram->AttachShader(gouraudVertexShader)) { return false; }
    if (!gouraudShaderProgram->AttachShader(gouraudFragmentShader)) { return false; }

    if (!simpleTeapotShaderProgram1->Link()) { return false; }
    if (!phongShaderProgram->Link()) { return false; }
    if (!gouraudShaderProgram->Link()) { return false; }
    
    if (Texture2D* texture = CreateTexture("pixel_diffuse")) { texture->LoadFromFile("assets/textures/pixel_diffuse.dds"); } else { return false; }
    if (Texture2D* texture = CreateTexture("bricks_diffuse")) { texture->LoadFromFile("assets/textures/bricks_diffuse.dds"); } else { return false; }
    if (Texture2D* texture = CreateTexture("bricks_specular")) { texture->LoadFromFile("assets/textures/bricks_specular.dds"); } else { return false; }
    if (Texture2D* texture = CreateTexture("wood_diffuse")) { texture->LoadFromFile("assets/textures/wood_texture.dds"); } else { return false; }

    CreateMaterial<PhongGouraudMaterial>("Cube", phongShaderProgram);
    CreateMaterial<PhongGouraudMaterial>("Cylinder", phongShaderProgram);
    CreateMaterial<PhongGouraudMaterial>("Sphere", phongShaderProgram);
    CreateMaterial<SimpleMaterial>("Torus", simpleTeapotShaderProgram1);

    CreateCubeMesh("Cube", 1.5f, 1.5f, 1.5f);
    CreateCylinderMesh("Cylinder", 32, 1.f, 1.3f);
    CreateSphereMesh("Sphere", 64, 32, 1.f);
    CreateTorusMesh("Torus", 32, 8, 4.5f, 0.5f);

    return true;
}

void AssetManager::Unload()
{
    DeleteShaderPrograms();
    DeleteShaders();
    DeleteMaterials();
    DeleteTextures();
    DeleteMeshes();
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

Mesh* AssetManager::GetMesh(std::string const& name)
{
    MeshMap::const_iterator itr = meshes.find(name);
    return itr != meshes.end() ? itr->second : nullptr;
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

void AssetManager::DeleteTextures()
{
    for (TextureMap::const_iterator itr = textures.begin(); itr != textures.end(); ++itr)
    {
        delete itr->second;
    }

    textures.clear();
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

void AssetManager::DeleteMeshes()
{
    for (MeshMap::const_iterator itr = meshes.begin(); itr != meshes.end(); ++itr)
    {
        delete itr->second;
    }

    meshes.clear();
}
