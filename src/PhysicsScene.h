#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "foundation/PxErrorCallback.h"
#include "foundation/PxErrors.h"

class Scene;

namespace physx
{
    class PxFoundation;
}

class PhysicsScene
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
    PhysicsScene(Scene* ownerScene);
    ~PhysicsScene();

    Scene* GetOwnerScene() const { return ownerScene; }

private:
    Scene* ownerScene;
    physx::PxFoundation* foundation;
    AllocatorCallback allocatorCallback;
    ErrorCallback errorCallback;
};
