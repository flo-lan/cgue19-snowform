#pragma once

#include "LightComponent.h"
#include <glm\glm.hpp>
#include <GL/glew.h>

class CameraComponent;
class ShadowMapMaterial;

class DirectionalLightComponent : public LightComponent
{
public:
    DirectionalLightComponent(GameObject* owner);
    virtual ~DirectionalLightComponent();

    void InitializeShadowMaps();
    void RenderShadowMaps(CameraComponent* camera);
    void ActivateAndBindShadowMap(int unit);

    glm::vec3 GetDirection() const;

    glm::vec3 GetColor() const { return color; }
    void SetColor(glm::vec3 value) { color = value; }

    float GetIntensity() const { return intensity; }
    void SetIntensity(float value) { intensity = value; }

    int GetShadowMapWidth() const { return (int)_ShadowMapWidth; }
    void SetShadowMapWidth(int shadowMapWidth) { _ShadowMapWidth = (GLsizei)shadowMapWidth; }

    int GetShadowMapHeight() const { return (int)_ShadowMapHeight; }
    void SetShadowMapHeight(int shadowMapHeight) { _ShadowMapHeight = (GLsizei)shadowMapHeight; }

    glm::mat4 const& GetShadowMapLightSpace() const { return _ShadowMapLightSpace; }

    bool IsShadowEnabled() const { return _ShadowEnabled; }
    void EnableShadow(bool enable);

private:
    glm::vec3 color;
    float intensity;
    GLuint _ShadowMapFBO;
    GLuint _ShadowMapDBO[NUM_DIRECTIONAL_SHADOW_CASCADES];
    GLsizei _ShadowMapWidth;
    GLsizei _ShadowMapHeight;
    ShadowMapMaterial* _ShadowMapMaterial;
    glm::mat4 _ShadowMapLightSpace;
    bool _ShadowInitialized;
    bool _ShadowEnabled;
};
