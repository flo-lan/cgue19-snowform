#pragma once

#include "Component.h"

class GameObject;

namespace physx
{
    class PxMaterial;
    class PxGeometry;
    class PxShape;
}

class ColliderComponent : public Component
{
public:
    ColliderComponent(GameObject* owner);
    virtual ~ColliderComponent();

    physx::PxMaterial* GetPxMaterial() const { return pxMaterial; }
    void SetPxMaterial(physx::PxMaterial* value);

protected:
    physx::PxGeometry* GetPxGeometry() const { return pxGeometry; }
    void SetPxGeometry(physx::PxGeometry* value);

private:
    void CreatePxShape();

    physx::PxMaterial* pxMaterial;
    physx::PxGeometry* pxGeometry;
    physx::PxShape* pxShape;
};
