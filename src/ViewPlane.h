#pragma once

#include <glm/glm.hpp>

class ViewPlane
{
public:
    ViewPlane();

    void SetThreePoints(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3);
    void SetNormalAndPoint(glm::vec3 const& normal, glm::vec3 const& point);
    float Distance(glm::vec3 const& p) const { return d + glm::dot(normal, p); }

    glm::vec3 GetNormal() const { return normal; }
    glm::vec3 GetPoint() const { return point; }

private:
    glm::vec3 normal;
    glm::vec3 point;
    float d;
};
