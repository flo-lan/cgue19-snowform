#pragma once

#include "LightComponent.h"
#include <glm\glm.hpp>
#include <GL/glew.h>

class CameraComponent;
class ShadowMapMaterial;

#define MAX_DIRECTIONAL_LIGHT_COUNT 1
#define NUM_DIRECTIONAL_SHADOW_CASCADES 3
#define NUM_FRUSTUM_CORNERS 8

class DirectionalLightComponent : public LightComponent
{
public:
    DirectionalLightComponent(GameObject* owner);
    virtual ~DirectionalLightComponent();

    void InitializeShadowMaps();
    void RenderShadowMaps(CameraComponent* camera);
    void ActivateAndBindShadowMap(int unit, int cascadeIndex);

    glm::vec3 GetDirection() const;

    glm::vec3 GetColor() const { return color; }
    void SetColor(glm::vec3 value) { color = value; }

    float GetIntensity() const { return intensity; }
    void SetIntensity(float value) { intensity = value; }

    int GetShadowMapWidth() const { return (int)_ShadowMapWidth; }
    void SetShadowMapWidth(int shadowMapWidth) { _ShadowMapWidth = (GLsizei)shadowMapWidth; }

    int GetShadowMapHeight() const { return (int)_ShadowMapHeight; }
    void SetShadowMapHeight(int shadowMapHeight) { _ShadowMapHeight = (GLsizei)shadowMapHeight; }

    glm::mat4 const& GetShadowMapProjection(int cascadeIndex) const { return _ShadowMapProjections[cascadeIndex]; }
    glm::vec4 const& GetShadowMapCascadeBounds(int cascadeIndex) const { return _ShadowMapCascadeBounds[cascadeIndex]; }

    bool IsShadowEnabled() const { return _ShadowEnabled; }
    void EnableShadow(bool enable);

private:
    TransformComponent* _FrustumDebugSpheres[8*3];
    glm::vec3 color;
    float intensity;
    GLuint _ShadowMapFBO;
    GLuint _ShadowMapDBO[NUM_DIRECTIONAL_SHADOW_CASCADES];
    GLsizei _ShadowMapWidth;
    GLsizei _ShadowMapHeight;
    ShadowMapMaterial* _ShadowMapMaterial;
    glm::mat4 _ShadowMapProjections[NUM_DIRECTIONAL_SHADOW_CASCADES];
    glm::vec4 _ShadowMapCascadeBounds[NUM_DIRECTIONAL_SHADOW_CASCADES];
    bool _ShadowInitialized;
    bool _ShadowEnabled;
};
