#include "DirectionalLightComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "ShadowMapMaterial.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp> // glm::ortho

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

    _ShadowMapProjections = glm::ortho
    (
        _ShadowMapOrthoLeft, _ShadowMapOrthoRight,
        _ShadowMapOrthoBottom, _ShadowMapOrthoTop,
        _ShadowMapOrthoNear, _ShadowMapOrthoFar
    ) *
    glm::lookAt
    (
        transform->GetPosition(),
        transform->GetPosition() + transform->GetDirectionBackward(),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    if (_ShadowMapMaterial)
    {
        glViewport(0, 0, _ShadowMapWidth, _ShadowMapHeight);

        glBindFramebuffer(GL_FRAMEBUFFER, _ShadowMapFBO);
        {
            glClear(GL_DEPTH_BUFFER_BIT);

            _ShadowMapMaterial->SetShadowMapProjection(_ShadowMapProjections);

            for (auto itr = affectedMeshRenderers.begin(); itr != affectedMeshRenderers.end(); ++itr)
            {
                if ((*itr)->IsCastShadowsEnabled())
                {
                    (*itr)->Render(camera, _ShadowMapMaterial);
                }
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
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
