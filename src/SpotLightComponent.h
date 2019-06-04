#pragma once

#include "LightComponent.h"
#include <glm\glm.hpp>

class SpotLightComponent : public LightComponent
{
public:
    SpotLightComponent(GameObject* owner);
    virtual ~SpotLightComponent();

    glm::vec3 GetPosition() const;
    glm::vec3 GetDirection() const { return direction; }
    glm::vec3 GetColor() const { return color; }
    float GetIntensity() const { return intensity; }
    float GetInnerCutOff() const { return innerCutOff; }
    float GetOuterCutOff() const { return outerCutOff; }
    float GetConstant() const { return constant; }
    float GetLinear() const { return linear; }
    float GetQuadratic() const { return quadratic; }

    void SetDirection(glm::vec3 value) { direction = value; }
    void SetColor(glm::vec3 value) { color = value; }
    void SetIntensity(float value) { intensity = value; }
    void SetInnerCutOff(float value) { innerCutOff = value; }
    void SetOuterCutOff(float value) { outerCutOff = value; }
    void SetConstant(float value) { constant = value; }
    void SetLinear(float value) { linear = value; }
    void SetQuadratic(float value) { quadratic = value; }

private:
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float innerCutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};
