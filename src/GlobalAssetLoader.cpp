#include "GlobalAssetLoader.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "SimpleMaterial.h"
#include "StandardMaterial.h"
#include "ImageMaterial.h"
#include "ImageCutOffMaterial.h"
#include "TextMaterial.h"
#include "TextMaterial3D.h"
#include "ShadowMapMaterial.h"
#include "ParticleBillboardMaterial.h"
#include "AssetManager.h"
#include "PhysicsEngine.h"
#include "Cubemap.h"
#include "SkyboxMaterial.h"

GlobalAssetLoader::GlobalAssetLoader()
{
}

GlobalAssetLoader::~GlobalAssetLoader()
{
}

bool GlobalAssetLoader::LoadAssets()
{
    ShaderProgram* simpleShaderProgram = nullptr;
    ShaderProgram* standardShaderProgram = nullptr;
    ShaderProgram* imageShaderProgram = nullptr;
    ShaderProgram* imageCutOffShaderProgram = nullptr;
    ShaderProgram* textShaderProgram = nullptr;
    ShaderProgram* shadowMapShaderProgram = nullptr;
    ShaderProgram* particleBillboardShaderProgram = nullptr;
    ShaderProgram* skyboxShaderProgram = nullptr;

    if (!(simpleShaderProgram = LoadShaderProgramFromFiles("simple_shader_program",
            "simple_vertex_shader", "assets/shaders/simple_vertex.glsl",
            "simple_fragment_shader", "assets/shaders/simple_fragment.glsl")) ||
        !(standardShaderProgram = LoadShaderProgramFromFiles("standard_shader_program",
            "standard_vertex_shader", "assets/shaders/standard_vertex.glsl",
            "standard_fragment_shader", "assets/shaders/standard_fragment.glsl")) ||
        !(imageShaderProgram = LoadShaderProgramFromFiles("image_shader_program",
            "image_vertex_shader", "assets/shaders/image_vertex.glsl",
            "image_fragment_shader", "assets/shaders/image_fragment.glsl")) ||
        !(imageCutOffShaderProgram = LoadShaderProgramFromFiles("image_cutoff_shader_program",
            "image_cutoff_vertex_shader", "assets/shaders/image_cutoff_vertex.glsl",
            "image_cutoff_fragment_shader", "assets/shaders/image_cutoff_fragment.glsl")) ||
        !(textShaderProgram = LoadShaderProgramFromFiles("text_shader_program",
            "text_vertex_shader", "assets/shaders/text_vertex.glsl",
            "text_fragment_shader", "assets/shaders/text_fragment.glsl")) ||
        !(shadowMapShaderProgram = LoadShaderProgramFromFiles("shadow_map_shader_program",
            "shadow_map_vertex_shader", "assets/shaders/shadow_map_vertex.glsl",
            "shadow_map_fragment_shader", "assets/shaders/shadow_map_fragment.glsl")) ||
        !(particleBillboardShaderProgram = LoadShaderProgramFromFiles("particle_billboard_shader_program",
            "particle_billboard_vertex_shader", "assets/shaders/particle_billboard_vertex.glsl",
            "particle_billboard_fragment_shader", "assets/shaders/particle_billboard_fragment.glsl")) ||
        !(skyboxShaderProgram = LoadShaderProgramFromFiles("skybox_shader_program",
            "skybox_vertex_shader", "assets/shaders/skybox_vertex.glsl",
            "skybox_fragment_shader", "assets/shaders/skybox_fragment.glsl")))
    {
        return false;
    }

    if (!LoadComputeShaderProgramFromFile("particle_system_compute_shader_program",
        "particle_system_compute_shader", "assets/shaders/particle_system_compute.glsl"))
    {
        return false;
    }

    if (!LoadTextureFromFile("pixel_diffuse", "assets/textures/pixel_diffuse.dds") ||
        !LoadTextureFromFile("bricks_diffuse", "assets/textures/bricks_diffuse.dds") ||
        !LoadTextureFromFile("bricks_specular", "assets/textures/bricks_specular.dds") ||
        !LoadTextureFromFile("wood_diffuse", "assets/textures/wood_texture.dds") ||
        !LoadTextureFromFile("snowball_diffuse", "assets/textures/snowball_diffuse.dds") ||
        !LoadTextureFromFile("arial_atlas", "assets/fonts/arial.dds", false /* Disable mip maps */) ||
        !LoadTextureFromFile("transition_overlay", "assets/textures/transition_overlay.dds", false /* Disable mip maps */) ||
        !LoadTextureFromFile("transition_overlay_cutoff", "assets/textures/transition_overlay_cutoff.dds", false /* Disable mip maps */))
    {
        return false;
    }

    if (Cubemap* skyboxCubemap = sAssetManager.CreateCubemap("Skybox"))
    {
        if (!skyboxCubemap->LoadFromFile("assets/textures/cubemap/negx.dds",
            "assets/textures/cubemap/posx.dds",
            "assets/textures/cubemap/posy.dds",
            "assets/textures/cubemap/negy.dds",
            "assets/textures/cubemap/posz.dds",
            "assets/textures/cubemap/negz.dds"))
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    if (!sAssetManager.CreateFontFromFile("Arial", "assets/fonts/arial.fnt", sAssetManager.GetTexture2D("arial_atlas")))
    {
        return false;
    }

    sAssetManager.CreateMaterial<SimpleMaterial>("SimpleDefault", simpleShaderProgram);
    sAssetManager.CreateMaterial<StandardMaterial>("StandardDefault", standardShaderProgram);
    sAssetManager.CreateMaterial<StandardMaterial>("Cube", standardShaderProgram);
    sAssetManager.CreateMaterial<StandardMaterial>("Cylinder", standardShaderProgram);
    sAssetManager.CreateMaterial<StandardMaterial>("Sphere", standardShaderProgram);
    sAssetManager.CreateMaterial<ShadowMapMaterial>("ShadowMapDefault", shadowMapShaderProgram);

    if (auto m = sAssetManager.CreateMaterial<StandardMaterial>("Snowball", standardShaderProgram))
    {
        m->SetDiffuseTexture(sAssetManager.GetTexture2D("snowball_diffuse"));
    }

    if (TextMaterial* tm = sAssetManager.CreateMaterial<TextMaterial>("CoinCount", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
    }

    if (TextMaterial* tm = sAssetManager.CreateMaterial<TextMaterial>("FPS", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.91f, 0.835f, 0.192f, 1.f));
    }

    if (TextMaterial* tm = sAssetManager.CreateMaterial<TextMaterial>("RemainingTime", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
    }

    if (TextMaterial* tm = sAssetManager.CreateMaterial<TextMaterial>("LevelFinished", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
    }

    if (TextMaterial* tm = sAssetManager.CreateMaterial<TextMaterial>("CoinsLeft", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
    }

    if (TextMaterial* tm = sAssetManager.CreateMaterial<TextMaterial>("Credits", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
    }

    if (TextMaterial* tm = sAssetManager.CreateMaterial<TextMaterial>("OutlineBlack", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
    }

    if (TextMaterial* tm = sAssetManager.CreateMaterial<TextMaterial>("OutlineDarkRed", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.8f, 0.f, 0.f, 1.f));
    }

    if (TextMaterial3D* tm = sAssetManager.CreateMaterial<TextMaterial3D>("OutlineBlack3D", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
    }

    if (TextMaterial3D* tm = sAssetManager.CreateMaterial<TextMaterial3D>("OutlineDarkRed3D", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.8f, 0.f, 0.f, 1.f));
    }

    if (TextMaterial3D* tm = sAssetManager.CreateMaterial<TextMaterial3D>("OutlineDarkYellow3D", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(0.91f, 0.835f, 0.192f, 1.f));
    }

    if (TextMaterial3D* tm = sAssetManager.CreateMaterial<TextMaterial3D>("SnowmanText", textShaderProgram))
    {
        tm->SetAtlasTexture(sAssetManager.GetTexture2D("arial_atlas"));
        tm->SetTextColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
    }

    if (ImageMaterial* im = sAssetManager.CreateMaterial<ImageMaterial>("Header", imageShaderProgram))
    {
        im->SetImageColor(glm::vec4(1.f, 1.f, 1.f, 0.5f));
    }

    if (ImageCutOffMaterial* im = sAssetManager.CreateMaterial<ImageCutOffMaterial>("TransitionOverlay", imageCutOffShaderProgram))
    {
        im->SetCutOffTexture(sAssetManager.GetTexture2D("transition_overlay_cutoff"));
        im->SetImageTexture(sAssetManager.GetTexture2D("transition_overlay"));
    }

    if (ParticleBillboardMaterial* pbm = sAssetManager.CreateMaterial<ParticleBillboardMaterial>("SnowParticle", particleBillboardShaderProgram))
    {
        pbm->SetDiffuseColor(glm::vec3(1.f));
    }

    if (SkyboxMaterial* sbm = sAssetManager.CreateMaterial<SkyboxMaterial>("Skybox", skyboxShaderProgram))
    {
        sbm->SetCubemap(sAssetManager.GetCubemap("Skybox"));
    }

    sAssetManager.CreateQuadMesh("Image", 1.f, 1.f);
    sAssetManager.CreateQuadMesh("SnowParticle", 1.f, 1.f);
    sAssetManager.CreateCubeMesh("Skybox", 1.f, 1.f, 1.f);
    sAssetManager.CreateCubeMesh("Cube", 1.5f, 1.5f, 1.5f);
    sAssetManager.CreateCylinderMesh("Cylinder", 32, 1.f, 1.3f);
    sAssetManager.CreateSphereMesh("Sphere", 64, 32, 1.f);
    sAssetManager.CreateTorusMesh("Torus", 32, 8, 4.5f, 0.5f);

    sAssetManager.CreateMeshFromFile("BlockMoving", "assets/meshes/block_moving.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnow", "assets/meshes/block_snow.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowCliff", "assets/meshes/block_snow_cliff.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowCliffCorner", "assets/meshes/block_snow_cliff_corner.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowCornerLarge", "assets/meshes/block_snow_corner_large.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowCornerSmall", "assets/meshes/block_snow_corner_small.fbx");
    sAssetManager.CreateMeshFromFile("BlockSnowSlope", "assets/meshes/block_snow_slope.fbx");
    sAssetManager.CreateMeshFromFile("BlockHexagon", "assets/meshes/block_hexagon.fbx");
    sAssetManager.CreateMeshFromFile("Coin", "assets/meshes/coin.fbx");
    sAssetManager.CreateMeshFromFile("Flag", "assets/meshes/flag.fbx");
    sAssetManager.CreateMeshFromFile("Mushrooms", "assets/meshes/mushrooms.fbx");
    sAssetManager.CreateMeshFromFile("Rocks", "assets/meshes/rocks.fbx");
    sAssetManager.CreateMeshFromFile("FenceCornerCurved", "assets/meshes/fence_corner_curved.fbx");
    sAssetManager.CreateMeshFromFile("Snowball", "assets/meshes/snowball.fbx");
    sAssetManager.CreateMeshFromFile("Snowman", "assets/meshes/snowman.fbx");
    sAssetManager.CreateMeshFromFile("TreePineSnow", "assets/meshes/tree_pine_snow.fbx");

    sPhysicsEngine.CreatePxMaterial("Default", 0.5f, 0.5f, 0.5f);
    sPhysicsEngine.CreatePxMaterial("Snowball", 5.0f, 5.0f, 0.5f);

    return true;
}

void GlobalAssetLoader::UnloadAssets()
{
    // No implementation needed, since asset manager will unload all assets on shutdown anyway
}
