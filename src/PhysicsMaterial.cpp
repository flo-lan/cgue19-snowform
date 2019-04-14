#include "PhysicsMaterial.h"
#include "PxMaterial.h"

PhysicsMaterial::PhysicsMaterial(std::string const& _name, physx::PxMaterial* _pxMaterial) :
    name(_name),
    pxMaterial(_pxMaterial)
{
    fprintf(stdout, "Created physics material '%s'!\n", name.c_str());
}

PhysicsMaterial::~PhysicsMaterial()
{
    if (pxMaterial)
    {
        pxMaterial->release();
        pxMaterial = nullptr;
    }

    fprintf(stdout, "Deleted physics material '%s'!\n", name.c_str());
}
