#include "GlobalAssetLoader.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "SimpleMaterial.h"
#include "StandardMaterial.h"
#include "AssetManager.h"
#include "PhysicsEngine.h"

GlobalAssetLoader::GlobalAssetLoader()
{
}

GlobalAssetLoader::~GlobalAssetLoader()
{
}

bool GlobalAssetLoader::LoadAssets()
{
    Shader* simpleVertexShader = sAssetManager.CreateShader("simple_vertex_shader", GL_VERTEX_SHADER);
    Shader* simpleFragmentShader = sAssetManager.CreateShader("simple_fragment_shader", GL_FRAGMENT_SHADER);
    Shader* standardVertexShader = sAssetManager.CreateShader("standard_vertex_shader", GL_VERTEX_SHADER);
    Shader* standardFragmentShader = sAssetManager.CreateShader("standard_fragment_shader", GL_FRAGMENT_SHADER);

    if (!simpleVertexShader) { return false; }
    if (!simpleFragmentShader) { return false; }
    if (!standardVertexShader) { return false; }
    if (!standardFragmentShader) { return false; }

    if (!simpleVertexShader->LoadSourceFromFile("assets/shaders/simple_vertex.glsl")) { return false; }
    if (!simpleFragmentShader->LoadSourceFromFile("assets/shaders/simple_fragment.glsl")) { return false; }
    if (!standardVertexShader->LoadSourceFromFile("assets/shaders/standard_vertex.glsl")) { return false; }
    if (!standardFragmentShader->LoadSourceFromFile("assets/shaders/standard_fragment.glsl")) { return false; }

    if (!simpleVertexShader->Compile()) { return false; }
    if (!simpleFragmentShader->Compile()) { return false; }
    if (!standardVertexShader->Compile()) { return false; }
    if (!standardFragmentShader->Compile()) { return false; }

    ShaderProgram* simpleShaderProgram = sAssetManager.CreateShaderProgram("simple_shader_program");
    ShaderProgram* standardShaderProgram = sAssetManager.CreateShaderProgram("standard_shader_program");

    if (!simpleShaderProgram) { return false; }
    if (!standardShaderProgram) { return false; }

    if (!simpleShaderProgram->AttachShader(simpleVertexShader)) { return false; }
    if (!simpleShaderProgram->AttachShader(simpleFragmentShader)) { return false; }
    if (!standardShaderProgram->AttachShader(standardVertexShader)) { return false; }
    if (!standardShaderProgram->AttachShader(standardFragmentShader)) { return false; }

    if (!simpleShaderProgram->Link()) { return false; }
    if (!standardShaderProgram->Link()) { return false; }

    if (Texture2D* texture = sAssetManager.CreateTexture("pixel_diffuse")) { texture->LoadFromFile("assets/textures/pixel_diffuse.dds"); }
    else { return false; }
    if (Texture2D* texture = sAssetManager.CreateTexture("bricks_diffuse")) { texture->LoadFromFile("assets/textures/bricks_diffuse.dds"); }
    else { return false; }
    if (Texture2D* texture = sAssetManager.CreateTexture("bricks_specular")) { texture->LoadFromFile("assets/textures/bricks_specular.dds"); }
    else { return false; }
    if (Texture2D* texture = sAssetManager.CreateTexture("wood_diffuse")) { texture->LoadFromFile("assets/textures/wood_texture.dds"); }
    else { return false; }
    if (Texture2D* texture = sAssetManager.CreateTexture("snowball_diffuse")) { texture->LoadFromFile("assets/textures/snowball_diffuse.dds"); }
    else { return false; }

    sAssetManager.CreateMaterial<SimpleMaterial>("SimpleDefault", simpleShaderProgram);
    sAssetManager.CreateMaterial<StandardMaterial>("StandardDefault", standardShaderProgram);
    sAssetManager.CreateMaterial<StandardMaterial>("Cube", standardShaderProgram);
    sAssetManager.CreateMaterial<StandardMaterial>("Cylinder", standardShaderProgram);
    sAssetManager.CreateMaterial<StandardMaterial>("Sphere", standardShaderProgram);
    auto snowballMaterial = sAssetManager.CreateMaterial<StandardMaterial>("Snowball", standardShaderProgram);
    snowballMaterial->SetDiffuseTexture(sAssetManager.GetTexture2D("snowball_diffuse"));

    sAssetManager.CreateCubeMesh("Cube", 1.5f, 1.5f, 1.5f);
    sAssetManager.CreateCylinderMesh("Cylinder", 32, 1.f, 1.3f);
    sAssetManager.CreateSphereMesh("Sphere", 64, 32, 1.f);
    sAssetManager.CreateTorusMesh("Torus", 32, 8, 4.5f, 0.5f);

    sAssetManager.CreateMeshFromFile("BlockSnow", "assets/meshes/block_snow.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowCliff", "assets/meshes/block_snow_cliff.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowCliffCorner", "assets/meshes/block_snow_cliff_corner.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowCornerLarge", "assets/meshes/block_snow_corner_large.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowCornerSmall", "assets/meshes/block_snow_corner_small.fbx");
    sAssetManager.CreateMeshFromFile("Snowball", "assets/meshes/snowball.fbx");
    sAssetManager.CreateMeshFromFile("TreePineSnow", "assets/meshes/tree_pine_snow.fbx");

    sPhysicsEngine.CreatePxMaterial("Default", 0.5f, 0.5f, 0.5f);

    return true;
}

void GlobalAssetLoader::UnloadAssets()
{
    // No implementation needed, since asset manager will unload all assets on shutdown anyway
}
