#pragma once

#include <string>

namespace physx
{
    class PxMaterial;
}

class PhysicsMaterial
{
public:
    PhysicsMaterial(std::string const& name, physx::PxMaterial* pxMaterial);
    ~PhysicsMaterial();

private:
    std::string name;
    physx::PxMaterial* pxMaterial;
};
