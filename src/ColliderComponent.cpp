#include "ColliderComponent.h"
#include "GameObject.h"
#include "PhysicsEngine.h"
#include "geometry/PxGeometry.h"
#include "PxMaterial.h"
#include "PxShape.h"

ColliderComponent::ColliderComponent(GameObject* owner) :
    Component::Component(owner),
    pxMaterial(nullptr),
    pxGeometry(nullptr),
    pxShape(nullptr)
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

    if (pxGeometry)
    {
        delete pxGeometry;
        pxGeometry = nullptr;
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

    if (pxGeometry)
    {
        delete pxGeometry;
    }

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

    if (pxShape)
    {
        // ToDo: Find rigid component and attach shape
    }
}
