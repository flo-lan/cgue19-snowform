#pragma once

#include "Scene.h"
#include "extensions/PxDefaultAllocator.h"
#include "foundation/PxErrorCallback.h"
#include "foundation/PxErrors.h"

namespace physx
{
    class PxFoundation;
}

class PhysXScene : public Scene
{
private:
    class AllocatorCallback : public physx::PxDefaultAllocator
    {
    };

    class ErrorCallback : public physx::PxErrorCallback
    {
        virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
    };

public:
    PhysXScene(std::string const& _name);
    virtual ~PhysXScene();

private:
    physx::PxFoundation* foundation;
    AllocatorCallback allocatorCallback;
    ErrorCallback errorCallback;
};
