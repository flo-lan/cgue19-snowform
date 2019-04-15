#include "MeshColliderComponent.h"
#include "GameObject.h"
#include "Mesh.h"
#include "PhysicsEngine.h"
#include "geometry/PxTriangleMesh.h"
#include "geometry/PxTriangleMeshGeometry.h"

MeshColliderComponent::MeshColliderComponent(GameObject* owner) :
    ColliderComponent::ColliderComponent(owner),
    pxTriangleMesh(nullptr)
{
}

MeshColliderComponent::~MeshColliderComponent()
{
}

void MeshColliderComponent::SetMesh(Mesh* value)
{
    if (!value)
    {
        return;
    }

    physx::PxTriangleMesh* pxTriangleMesh = sPhysicsEngine.GetPxTriangleMesh(value->Name);

    if (!pxTriangleMesh)
    {
        fprintf(stdout, "Warning: PhysX triangle mesh '%s' was created in mesh collider component. Consider preloading it in asset loader!\n", value->Name.c_str());
        pxTriangleMesh = sPhysicsEngine.CreatePxTriangleMesh(value->Name, value->Vertices, value->Indices);
    }

    SetMesh(pxTriangleMesh);
}

void MeshColliderComponent::SetMesh(physx::PxTriangleMesh* value)
{
    if (!value)
    {
        return;
    }

    if (value == pxTriangleMesh)
    {
        return;
    }

    pxTriangleMesh = value;

    SetPxGeometry(new physx::PxTriangleMeshGeometry(pxTriangleMesh));
}
