#pragma once

#include "LightComponent.h"
#include <glm\glm.hpp>
#include <GL/glew.h>

class ShadowMapMaterial;

#define MAX_DIRECTIONAL_LIGHT_COUNT 4

class DirectionalLightComponent : public LightComponent
{
public:
    DirectionalLightComponent(GameObject* owner);
    virtual ~DirectionalLightComponent();

    void InitializeShadowMap();
    void RenderShadowMap();
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

    float GetShadowMapOrthoLeft() const { return _ShadowMapOrthoLeft; }
    void SetShadowMapOrthoLeft(float shadowMapOrthoLeft) { _ShadowMapOrthoLeft = shadowMapOrthoLeft; }

    float GetShadowMapOrthoRight() const { return _ShadowMapOrthoRight; }
    void SetShadowMapOrthoRight(float shadowMapOrthoRight) { _ShadowMapOrthoRight = shadowMapOrthoRight; }

    float GetShadowMapOrthoBottom() const { return _ShadowMapOrthoBottom; }
    void SetShadowMapOrthoBottom(float shadowMapOrthoBottom) { _ShadowMapOrthoBottom = shadowMapOrthoBottom; }

    float GetShadowMapOrthoTop() const { return _ShadowMapOrthoTop; }
    void SetShadowMapOrthoTop(float shadowMapOrthoTop) { _ShadowMapOrthoTop = shadowMapOrthoTop; }

    float GetShadowMapOrthoNear() const { return _ShadowMapOrthoNear; }
    void SetShadowMapOrthoNear(float shadowMapOrthoNear) { _ShadowMapOrthoNear = shadowMapOrthoNear; }

    float GetShadowMapOrthoFar() const { return _ShadowMapOrthoFar; }
    void SetShadowMapOrthoFar(float shadowMapOrthoFar) { _ShadowMapOrthoFar = shadowMapOrthoFar; }

    glm::mat4 const& GetShadowMapLightSpace() const { return _ShadowMapLightSpace; }

    bool IsShadowEnabled() const { return _ShadowEnabled; }
    void EnableShadow(bool enable);

private:
    glm::vec3 color;
    float intensity;
    GLuint _ShadowMapFBO; // Frame Buffer Object
    GLuint _ShadowMapDBO; // Depth Buffer Object
    GLsizei _ShadowMapWidth;
    GLsizei _ShadowMapHeight;
    float _ShadowMapOrthoLeft;
    float _ShadowMapOrthoRight;
    float _ShadowMapOrthoBottom;
    float _ShadowMapOrthoTop;
    float _ShadowMapOrthoNear;
    float _ShadowMapOrthoFar;
    ShadowMapMaterial* _ShadowMapMaterial;
    glm::mat4 _ShadowMapLightSpace;
    bool _ShadowInitialized;
    bool _ShadowEnabled;
};
