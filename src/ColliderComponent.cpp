#include "ColliderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PhysicsEngine.h"
#include "RigidDynamicComponent.h"
#include "RigidStaticComponent.h"
#include "geometry/PxGeometry.h"
#include "geometry/PxTriangleMeshGeometry.h"
#include "geometry/PxBoxGeometry.h"
#include "geometry/PxSphereGeometry.h"
#include "geometry/PxPlaneGeometry.h"
#include "geometry/PxCapsuleGeometry.h"
#include "PxMaterial.h"
#include "PxShape.h"
#include <iostream>
#include <assert.h>

ColliderComponent::ColliderComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    pxMaterial(nullptr),
    pxGeometry(nullptr),
    pxShape(nullptr),
    pxShapeFlags(physx::PxShapeFlag::eSIMULATION_SHAPE | physx::PxShapeFlag::eSCENE_QUERY_SHAPE)
{
}

ColliderComponent::~ColliderComponent()
{
    if (pxShape)
    {
        pxShape->release();
        pxShape = nullptr;
    }

    // Do not release pxMaterial - will be deleted from asset manager on shutdown
    pxMaterial = nullptr;

    DeletePxGeometry();
}

void ColliderComponent::SetTrigger(bool value)
{
    if (value)
    {
        SetPxShapeFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
        SetPxShapeFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
    }
    else
    {
        SetPxShapeFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
        SetPxShapeFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    }
}

void ColliderComponent::SetPxMaterial(physx::PxMaterial* value)
{
    if (!value)
    {
        return;
    }

    if (value == pxMaterial)
    {
        return;
    }

    pxMaterial = value;

    if (pxShape)
    {
        pxShape->setMaterials(&pxMaterial, 1);
    }
    else
    {
        CreatePxShape();
    }
}

void ColliderComponent::SetPxGeometry(physx::PxGeometry* value)
{
    if (!value)
    {
        return;
    }

    if (value == pxGeometry)
    {
        return;
    }

    DeletePxGeometry();
    pxGeometry = value;

    if (pxShape)
    {
        pxShape->setGeometry(*value);
    }
    else
    {
        CreatePxShape();
    }
}

void ColliderComponent::SetPxShapeFlag(physx::PxShapeFlag::Enum flag, bool value)
{
    if (value)
    {
        if (pxShapeFlags & flag)
        {
            // Flag already set
            return;
        }

        pxShapeFlags |= flag;
    }
    else
    {
        if (!(pxShapeFlags & flag))
        {
            // Flag not set
            return;
        }

        pxShapeFlags &= ~flag;
    }

    if (pxShape)
    {
        pxShape->setFlag(flag, value);
    }
}

void ColliderComponent::SetPxShapeFlags(physx::PxShapeFlags flags)
{
    if (pxShapeFlags == flags)
    {
        return;
    }

    pxShapeFlags = flags;

    if (pxShape)
    {
        pxShape->setFlags(flags);
    }
}

void ColliderComponent::CreatePxShape()
{
    if (pxShape)
    {
        return;
    }

    if (!pxMaterial)
    {
        return;
    }

    if (!pxGeometry)
    {
        return;
    }

    pxShape = sPhysicsEngine.CreatePxShape(*pxGeometry, *pxMaterial);

    if (!pxShape)
    {
        return;
    }

    pxShape->setFlags(pxShapeFlags);

    if (RigidDynamicComponent* rigidDynamic = GetOwner()->GetComponent<RigidDynamicComponent>())
    {
        rigidDynamic->AttachColliderComponent(this);
    }
    else if (RigidStaticComponent* rigidStatic = GetOwner()->GetComponent<RigidStaticComponent>())
    {
        rigidStatic->AttachColliderComponent(this);
    }
}

void ColliderComponent::DeletePxGeometry()
{
    if (!pxGeometry)
    {
        return;
    }

    switch (pxGeometry->getType())
    {
        case physx::PxGeometryType::Enum::eBOX:
        {
            physx::PxBoxGeometry* pxBoxGeometry = (physx::PxBoxGeometry*)pxGeometry;
            delete pxBoxGeometry;
            break;
        }
        case physx::PxGeometryType::Enum::eSPHERE:
        {
            physx::PxSphereGeometry* pxSphereGeometry = (physx::PxSphereGeometry*)pxGeometry;
            delete pxSphereGeometry;
            break;
        }
        case physx::PxGeometryType::Enum::ePLANE:
        {
            physx::PxPlaneGeometry* pxPlaneGeometry = (physx::PxPlaneGeometry*)pxGeometry;
            delete pxPlaneGeometry;
            break;
        }
        case physx::PxGeometryType::Enum::eCAPSULE:
        {
            physx::PxCapsuleGeometry* pxCapsuleGeometry = (physx::PxCapsuleGeometry*)pxGeometry;
            delete pxCapsuleGeometry;
            break;
        }
        case physx::PxGeometryType::Enum::eTRIANGLEMESH:
        {
            physx::PxTriangleMeshGeometry* pxTriangleMeshGeometry = (physx::PxTriangleMeshGeometry*)pxGeometry;
            delete pxTriangleMeshGeometry;
            break;
        }
        default:
        {
            fprintf(stderr, "Could not delete PhysX geometry type %i, because deleting of this type is not implemented!", ((int)pxGeometry->getType()));
            assert(false);
            break;
        }
    }

    pxGeometry = nullptr;
}
