#pragma once

#include "Component.h"
#include <glm\glm.hpp>

#define MAX_DIRECTIONAL_LIGHT_COUNT 4

class DirectionalLightComponent : public Component
{
public:
    DirectionalLightComponent(GameObject* owner);
    virtual ~DirectionalLightComponent();

    glm::vec3 GetDirection() const { return direction; }
    glm::vec3 GetColor() const { return color; }
    float GetIntensity() const { return intensity; }

    void SetDirection(glm::vec3 value) { direction = value; }
    void SetColor(glm::vec3 value) { color = value; }
    void SetIntensity(float value) { intensity = value; }

private:
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};
