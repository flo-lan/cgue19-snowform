#pragma once

#include <glm/glm.hpp>

struct Bounds
{
    Bounds() :
        Center(glm::vec3(0.f)),
        Size(glm::vec3(0.f)),
        Min(glm::vec3(0.f)),
        Max(glm::vec3(0.f))
    {}

    void SetMinMax(glm::vec3 const& min, glm::vec3 const& max);
    void SetCenter(glm::vec3 const& center);
    void Scale(glm::vec3 const& scale);

    glm::vec3 GetCornerP(glm::vec3 const& normal) const;
    glm::vec3 GetCornerN(glm::vec3 const& normal) const;

    glm::vec3 Center; // The center of the bounding box.
    glm::vec3 Size;   // The total size of the box.
    glm::vec3 Min;    // The minimal point of the box.
    glm::vec3 Max;    // The maximal point of the box.
};
