#include "ViewPlane.h"

// Credits: http://cgvr.cs.uni-bremen.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/

ViewPlane::ViewPlane() :
    normal(0.f),
    point(0.f),
    d(0.f)
{
}

void ViewPlane::SetThreePoints(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3)
{
    glm::vec3 aux1 = v1 - v2;
    glm::vec3 aux2 = v3 - v2;

    normal = glm::normalize(glm::cross(aux2, aux1));
    point = v2;

    d = -glm::dot(normal, point);
}

void ViewPlane::SetNormalAndPoint(glm::vec3 const& normal, glm::vec3 const& point)
{
    this->normal = glm::normalize(normal);
    this->point = point;

    d = -glm::dot(this->normal, this->point);
}
