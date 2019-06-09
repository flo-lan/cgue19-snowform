#include "DirectionalLightComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "ShadowMapMaterial.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "Screen.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp> // glm::ortho
#include <algorithm>
#include "BoxColliderComponent.h" // DEBUG
#include "RigidStaticComponent.h" // DEBUG
#include "PhysicsEngine.h" // DEBUG
#include "glm/ext.hpp" // DEBUG

DirectionalLightComponent::DirectionalLightComponent(GameObject* owner) :
    LightComponent::LightComponent(owner),
    color(1.f, 1.f, 1.f),
    intensity(1.f),
    _ShadowMapFBO(0),
    _ShadowMapDBO{0},
    _ShadowMapWidth(1024),
    _ShadowMapHeight(1024),
    _ShadowMapMaterial(sAssetManager.GetMaterial<ShadowMapMaterial>("ShadowMapDefault")),
    _ShadowMapProjections{glm::mat4(1.f)},
    _ShadowMapBounds{glm::vec4(0.f)},
    _ShadowInitialized(false),
    _ShadowEnabled(false)
{
    GetOwner()->GetScene()->InsertDirectionalLightComponent(this);

    // DEBUG
    for (int i = 0; i < 8 * 3; i++)
    {
        GameObject* frustumDebugSphere = GetOwner()->GetScene()->CreateGameObject("FrustumDebugSphere" + i);
        MeshRendererComponent* meshRenderer = frustumDebugSphere->AttachComponent<MeshRendererComponent>();
        meshRenderer->SetMaterial(sAssetManager.GetMaterial("Sphere"));
        meshRenderer->SetMesh(sAssetManager.GetMesh("Sphere"));
        frustumDebugSphere->AttachComponent<BoxColliderComponent>()->SetPxMaterial(sPhysicsEngine.GetPxMaterial("Default"));
        frustumDebugSphere->AttachComponent<RigidStaticComponent>();
        _FrustumDebugSpheres[i] = frustumDebugSphere->GetComponent<TransformComponent>();
    }
}

DirectionalLightComponent::~DirectionalLightComponent()
{
    if (_ShadowInitialized)
    {
        glDeleteTextures(NUM_DIRECTIONAL_SHADOW_CASCADES, _ShadowMapDBO);
        glDeleteFramebuffers(1, &_ShadowMapFBO);
    }

    while (affectedMeshRenderers.size())
    {
        affectedMeshRenderers[0]->RemoveLight(this);
    }

    GetOwner()->GetScene()->RemoveDirectionalLightComponent(this);
}

void DirectionalLightComponent::InitializeShadowMaps()
{
    if (_ShadowInitialized)
    {
        glDeleteTextures(NUM_DIRECTIONAL_SHADOW_CASCADES, _ShadowMapDBO);
        glDeleteFramebuffers(1, &_ShadowMapFBO);
    }

    // Create framebuffer object for rendering the depth texture
    glGenFramebuffers(1, &_ShadowMapFBO);

    // Create 2D textures that we will be used as the framebuffers depth textures
    glGenTextures(NUM_DIRECTIONAL_SHADOW_CASCADES, _ShadowMapDBO);

    for (int i = 0; i < NUM_DIRECTIONAL_SHADOW_CASCADES; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, _ShadowMapDBO[i]);
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                _ShadowMapWidth, _ShadowMapHeight,
                0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

            static float BorderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BorderColor);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, _ShadowMapFBO);
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _ShadowMapDBO[0], 0);
        glDrawBuffer(GL_NONE); // Tell OpenGL we are not going to render any color data
        glReadBuffer(GL_NONE); // Tell OpenGL we are not going to render any color data

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            fprintf(stderr, "Error on framebuffer initialization: 0x%x\n", status);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _ShadowInitialized = true;

    fprintf(stdout, "Initialized directional shadow maps for directional light component of game object '%s'!\n", GetOwner()->GetName().c_str());
}

void DirectionalLightComponent::RenderShadowMaps(CameraComponent* camera)
{
    if (!_ShadowMapMaterial)
    {
        return;
    }

    float _ShadowMapCascadeEnds[NUM_DIRECTIONAL_SHADOW_CASCADES + 1] =
    {
        -camera->GetNearPlane(),
        -camera->GetNearPlane() - (camera->GetFarPlane() - camera->GetNearPlane()) * 0.2f,
        -camera->GetNearPlane() - (camera->GetFarPlane() - camera->GetNearPlane()) * 0.6f,
        -camera->GetFarPlane()
    };

    glm::vec3 cameraPosition = camera->GetTransform()->GetPosition();
    glm::mat4 cameraMatrix = camera->GetTransform()->GetModelMatrix();
    glm::mat4 dLightMatrix = glm::lookAt(glm::vec3(0.f), -transform->GetDirectionBackward(), glm::vec3(0.f, 1.f, 0.f));

    float tanHalfHFOV = tanf(camera->GetFovInRadians() / 2.f * camera->GetAspectRatio());
    float tanHalfVFOV = tanf(camera->GetFovInRadians() / 2.f);

    glm::vec4 frustum[8];

    glViewport(0, 0, _ShadowMapWidth, _ShadowMapHeight);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _ShadowMapFBO);

    for (int i = 0; i < NUM_DIRECTIONAL_SHADOW_CASCADES; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _ShadowMapDBO[i], 0);

        glClear(GL_DEPTH_BUFFER_BIT);

        float xn = _ShadowMapCascadeEnds[i]     * tanHalfHFOV;
        float xf = _ShadowMapCascadeEnds[i + 1] * tanHalfHFOV;
        float yn = _ShadowMapCascadeEnds[i]     * tanHalfVFOV;
        float yf = _ShadowMapCascadeEnds[i + 1] * tanHalfVFOV;

        // Near
        frustum[0] = glm::vec4( xn,  yn, _ShadowMapCascadeEnds[i], 1.f);
        frustum[1] = glm::vec4(-xn,  yn, _ShadowMapCascadeEnds[i], 1.f);
        frustum[2] = glm::vec4( xn, -yn, _ShadowMapCascadeEnds[i], 1.f);
        frustum[3] = glm::vec4(-xn, -yn, _ShadowMapCascadeEnds[i], 1.f);

        // Far
        frustum[4] = glm::vec4( xf,  yf, _ShadowMapCascadeEnds[i + 1], 1.f);
        frustum[5] = glm::vec4(-xf,  yf, _ShadowMapCascadeEnds[i + 1], 1.f);
        frustum[6] = glm::vec4( xf, -yf, _ShadowMapCascadeEnds[i + 1], 1.f);
        frustum[7] = glm::vec4(-xf, -yf, _ShadowMapCascadeEnds[i + 1], 1.f);

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();

        for (int j = 0; j < NUM_FRUSTUM_CORNERS; ++j)
        {
            glm::vec4 frustumCorner = cameraMatrix * frustum[j];

            minX = std::min(minX, frustumCorner.x);
            maxX = std::max(maxX, frustumCorner.x);
            minY = std::min(minY, frustumCorner.y);
            maxY = std::max(maxY, frustumCorner.y);
            minZ = std::min(minZ, frustumCorner.z);
            maxZ = std::max(maxZ, frustumCorner.z);

            if (i == 0)
                _FrustumDebugSpheres[i*NUM_FRUSTUM_CORNERS + j]->SetLocalPosition(frustumCorner.x, frustumCorner.y, frustumCorner.z);
        }

        glm::vec3 center = glm::vec3
        (
            (maxX + minX) / 2.f,
            (maxY + minY) / 2.f,
            (maxZ + minZ) / 2.f
        );

        float radius = std::max(maxX - minX, maxZ - minZ) / 2.f;

        glm::vec3 lightPosition = center - transform->GetDirectionBackward() * radius;
        glm::mat4 lightMatrix = glm::lookAt(lightPosition, center, glm::vec3(0.f, 1.f, 0.f));

        for (int j = 0; j < NUM_FRUSTUM_CORNERS; ++j)
        {
            _FrustumDebugSpheres[i*NUM_FRUSTUM_CORNERS + j]->SetLocalPosition(lightPosition.x, lightPosition.y, lightPosition.z);
            _FrustumDebugSpheres[i*NUM_FRUSTUM_CORNERS + j]->LookAt(center);
        }

        float zFar = (i == 0) ? 2.5f : 2.f;

        _ShadowMapProjections[i] = glm::ortho(-radius, radius, -radius, radius, 1.f, radius * zFar) * lightMatrix;
        _ShadowMapBounds[i] = glm::vec4(minX + 0.1f, maxX - 0.1f, minZ + 0.1f, maxZ - 0.1f); // Make bounds a little smaller to avoid border artefacts

        _ShadowMapMaterial->SetShadowMapProjection(_ShadowMapProjections[i]);

        for (auto itr = affectedMeshRenderers.begin(); itr != affectedMeshRenderers.end(); ++itr)
        {
            (*itr)->Render(camera, _ShadowMapMaterial);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLightComponent::ActivateAndBindShadowMap(int unit, int cascadeIndex)
{
    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + unit);
    // Bind shadow map to activated texture unit
    glBindTexture(GL_TEXTURE_2D, _ShadowMapDBO[cascadeIndex]);
}

glm::vec3 DirectionalLightComponent::GetDirection() const
{
    return transform->GetDirectionBackward();
}

void DirectionalLightComponent::EnableShadow(bool enable)
{
    if (_ShadowEnabled == enable)
    {
        return;
    }

    if (enable && !_ShadowInitialized)
    {
        InitializeShadowMaps();
    }

    _ShadowEnabled = enable;
}
