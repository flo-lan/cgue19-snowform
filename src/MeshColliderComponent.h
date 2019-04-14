#pragma once

#include "ColliderComponent.h"

class Mesh;

namespace physx
{
    class PxTriangleMesh;
}

class MeshColliderComponent : public ColliderComponent
{
public:
    MeshColliderComponent(GameObject* owner);
    virtual ~MeshColliderComponent();

    void SetMesh(Mesh* value);
    void SetMesh(physx::PxTriangleMesh* value);

private:
    physx::PxTriangleMesh* pxTriangleMesh;
};
