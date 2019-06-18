#include "Bounds.h"

void Bounds::SetMinMax(glm::vec3 const& min, glm::vec3 const& max)
{
    Center = (max + min) / 2.f;
    Size = max - min;
    Min = min;
    Max = max;
}

void Bounds::Offset(glm::vec3 const& offset)
{
    Center += offset;
    // Size does not change
    Min += offset;
    Max += offset;
}

void Bounds::Scale(glm::vec3 const& scale)
{
    // Center does not change
    Size *= scale;
    Min = Center - Size * 0.5f;
    Max = Center + Size * 0.5f;
}

glm::vec3 Bounds::GetCornerP(glm::vec3 const& normal) const
{
    glm::vec3 corner = Min;

    if (normal.x >= 0) corner.x = Max.x;
    if (normal.y >= 0) corner.y = Max.y;
    if (normal.z >= 0) corner.z = Max.z;

    return corner;
}

glm::vec3 Bounds::GetCornerN(glm::vec3 const& normal) const
{
    glm::vec3 corner = Max;

    if (normal.x >= 0) corner.x = Min.x;
    if (normal.y >= 0) corner.y = Min.y;
    if (normal.z >= 0) corner.z = Min.z;

    return corner;
}
