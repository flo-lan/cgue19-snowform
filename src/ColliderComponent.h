#pragma once

#include "Component.h"
#include "PxShape.h"
#include <glm/glm.hpp>

class GameObject;
class TransformComponent;

namespace physx
{
    class PxMaterial;
    class PxGeometry;
    class PxShape;
}

class ColliderComponent : public Component
{
protected:
    ColliderComponent(GameObject* owner);

public:
    virtual ~ColliderComponent();

    void SetTrigger(bool value);
    bool IsTrigger() const { return pxShapeFlags & physx::PxShapeFlag::eTRIGGER_SHAPE; }

    void SetOffset(glm::vec3 const& value);
    glm::vec3 const& GetOffset() const { return offset; }

    physx::PxMaterial* GetPxMaterial() const { return pxMaterial; }
    void SetPxMaterial(physx::PxMaterial* value);

    physx::PxShape* GetPxShape() const { return pxShape; }

    physx::PxShapeFlags GetPxShapeFlags() const { return pxShapeFlags; }
    void SetPxShapeFlag(physx::PxShapeFlag::Enum flag, bool value);
    void SetPxShapeFlags(physx::PxShapeFlags flags);

protected:
    physx::PxGeometry* GetPxGeometry() const { return pxGeometry; }
    void SetPxGeometry(physx::PxGeometry* value);

    TransformComponent* transform;

private:
    void CreatePxShape();
    void DeletePxGeometry();

    physx::PxMaterial* pxMaterial;
    physx::PxGeometry* pxGeometry;
    physx::PxShape* pxShape;
    physx::PxShapeFlags pxShapeFlags;

    glm::vec3 offset;
};
