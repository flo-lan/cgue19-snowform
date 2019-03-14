#pragma once

#include "Component.h"
#include <glm\glm.hpp>

class TransformComponent;

#define MAX_POINT_LIGHT_COUNT 4

class PointLightComponent : public Component
{
public:
    PointLightComponent(GameObject* owner);
    virtual ~PointLightComponent();

    virtual void OnStart();

    glm::vec3 GetPosition() const;
    glm::vec3 GetColor() const { return color; }
    float GetIntensity() const { return intensity; }
    float GetConstant() const { return constant; }
    float GetLinear() const { return linear; }
    float GetQuadratic() const { return quadratic; }

    void SetColor(glm::vec3 value) { color = value; }
    void SetIntensity(float value) { intensity = value; }
    void SetConstant(float value) { constant = value; }
    void SetLinear(float value) { linear = value; }
    void SetQuadratic(float value) { quadratic = value; }

private:
    TransformComponent* transform;
    glm::vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};
