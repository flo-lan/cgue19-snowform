#pragma once

#include "Scene.h"
#include "physx/PxPhysicsAPI.h"

class PhysXScene : public Scene
{
public:
    PhysXScene(std::string const& _name);
    physx::PxDefaultErrorCallback gDefaultErrorCallback;
    physx::PxDefaultAllocator gDefaultAllocatorCallback;
private:
    physx::PxFoundation* mFoundation;
};
