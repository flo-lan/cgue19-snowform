#pragma once

#include "ViewPlane.h"
#include "Bounds.h"
#include <glm/glm.hpp>

enum
{
    VIEW_FRUSTUM_TOP = 0,
    VIEW_FRUSTUM_BOTTOM,
    VIEW_FRUSTUM_LEFT,
    VIEW_FRUSTUM_RIGHT,
    VIEW_FRUSTUM_NEARP,
    VIEW_FRUSTUM_FARP,
};

enum ViewFrustumObjectLocation
{
    VIEW_FRUSTUM_OUTSIDE = 0,
    VIEW_FRUSTUM_INTERSECT,
    VIEW_FRUSTUM_INSIDE
};

class ViewFrustum
{
public:
    ViewFrustum();

    void UpdateViewFrustum(glm::vec3 const& p, glm::vec3 const& l, glm::vec3 const& u);

    ViewFrustumObjectLocation BoundsInFrustum(Bounds const& bounds) const;

    void SetFovInDegrees(float value) { fovInRadians = glm::radians(value); }
    void SetAspectRatio(float value) { aspectRatio = value; }
    void SetNearPlane(float value) { nearPlane = value; }
    void SetFarPlane(float value) { farPlane = value; }

    float GetFovInRadians() const { return fovInRadians; }
    float GetAspectRatio() const { return aspectRatio; }
    float GetNearPlane() const { return nearPlane; }
    float GetFarPlane() const { return farPlane; }

private:
    ViewPlane viewPlanes[6];
    float fovInRadians;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    // Debug
    glm::vec3 ntl;
    glm::vec3 ntr;
    glm::vec3 nbl;
    glm::vec3 nbr;
    glm::vec3 ftl;
    glm::vec3 ftr;
    glm::vec3 fbl;
    glm::vec3 fbr;
};
