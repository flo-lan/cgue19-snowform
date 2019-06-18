#include "ViewFrustum.h"
#include "Screen.h"

// Credits: http://cgvr.cs.uni-bremen.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/

ViewFrustum::ViewFrustum() :
    viewPlanes{ViewPlane()},
    fovInRadians(glm::radians(60.f)),
    aspectRatio(Screen::GetWidth() / (float)Screen::GetHeight()),
    nearPlane(0.1f),
    farPlane(100.f),
    ntl(0.f),
    ntr(0.f),
    nbl(0.f),
    nbr(0.f),
    ftl(0.f),
    ftr(0.f),
    fbl(0.f),
    fbr(0.f)
{
}

void ViewFrustum::UpdateViewFrustum(glm::vec3 const& p, glm::vec3 const& l, glm::vec3 const& u)
{
    float tang = tanf(fovInRadians * 0.5f);
    float nh = nearPlane * tang;
    float nw = nh * aspectRatio;
    float fh = farPlane * tang;
    float fw = fh * aspectRatio;

    glm::vec3 Z = glm::normalize(p - l);            // compute the Z axis of camera
    glm::vec3 X = glm::normalize(glm::cross(u, Z)); // X axis of camera of given "up" vector and Z axis
    glm::vec3 Y = glm::cross(Z, X);                 // the real "up" vector is the cross product of Z and X

    glm::vec3 nc = p - Z * nearPlane;
    glm::vec3 fc = p - Z * farPlane;

    //ntl = nc + Y * nh - X * nw;
    //ntr = nc + Y * nh + X * nw;
    //nbl = nc - Y * nh - X * nw;
    //nbr = nc - Y * nh + X * nw;
    //
    //ftl = fc + Y * fh - X * fw;
    //ftr = fc + Y * fh + X * fw;
    //fbl = fc - Y * fh - X * fw;
    //fbr = fc - Y * fh + X * fw;
    //
    //viewPlanes[VIEW_FRUSTUM_TOP].SetThreePoints(ntr, ntl, ftl);
    //viewPlanes[VIEW_FRUSTUM_BOTTOM].SetThreePoints(nbl, nbr, fbr);
    //viewPlanes[VIEW_FRUSTUM_LEFT].SetThreePoints(ntl, nbl, fbl);
    //viewPlanes[VIEW_FRUSTUM_RIGHT].SetThreePoints(nbr, ntr, fbr);
    //viewPlanes[VIEW_FRUSTUM_NEARP].SetThreePoints(ntl, ntr, nbr);
    //viewPlanes[VIEW_FRUSTUM_FARP].SetThreePoints(ftr, ftl, fbl);

    viewPlanes[VIEW_FRUSTUM_NEARP].SetNormalAndPoint(-Z, nc);
    viewPlanes[VIEW_FRUSTUM_FARP].SetNormalAndPoint(Z, fc);

    glm::vec3 aux = glm::normalize((nc + Y * nh) - p);
    glm::vec3 normal = glm::cross(aux, X);
    viewPlanes[VIEW_FRUSTUM_TOP].SetNormalAndPoint(normal, nc + Y * nh);

    aux = glm::normalize((nc - Y * nh) - p);
    normal = glm::cross(X, aux);
    viewPlanes[VIEW_FRUSTUM_BOTTOM].SetNormalAndPoint(normal, nc - Y * nh);

    aux = glm::normalize((nc - X * nw) - p);
    normal = glm::cross(aux, Y);
    viewPlanes[VIEW_FRUSTUM_LEFT].SetNormalAndPoint(normal, nc - X * nw);

    aux = glm::normalize((nc + X * nw) - p);
    normal = glm::cross(Y, aux);
    viewPlanes[VIEW_FRUSTUM_RIGHT].SetNormalAndPoint(normal, nc + X * nw);
}

ViewFrustumObjectLocation ViewFrustum::BoundsInFrustum(Bounds const& bounds) const
{
    ViewFrustumObjectLocation result = VIEW_FRUSTUM_INSIDE;

    for (int i = 0; i < 6; ++i)
    {
        ViewPlane const& viewPlane = viewPlanes[i];

        if (viewPlane.Distance(bounds.GetCornerP(viewPlane.GetNormal())) < 0)
        {
            return VIEW_FRUSTUM_OUTSIDE;
        }

        if (viewPlane.Distance(bounds.GetCornerN(viewPlane.GetNormal())) < 0)
        {
            result = VIEW_FRUSTUM_INTERSECT;
        }
    }

    return result;
}
