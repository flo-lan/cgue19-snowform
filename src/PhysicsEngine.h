#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "foundation/PxErrorCallback.h"
#include "foundation/PxErrors.h"

#include <string>

class PhysicsMaterial;

namespace physx
{
    class PxFoundation;
    class PxPhysics;
    class PxScene;
    class PxDefaultCpuDispatcher;
}

class PhysicsEngine
{
public:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    static PhysicsEngine& getInstance()
    {
        static PhysicsEngine instance;
        return instance;
    }

public:
    // We want to make sure these two methods are unacceptable,
    // otherwise we may accidentally get copies of our singleton
    PhysicsEngine(PhysicsEngine const&) = delete;
    void operator=(PhysicsEngine const&) = delete;

    bool Start();
    void Update();
    void Stop();

    PhysicsMaterial* CreatePhysicsMaterial(std::string const& name,
        float staticFriction, float dynamicFriction, float restitution);

private:
    class AllocatorCallback : public physx::PxDefaultAllocator
    {
    };

    class ErrorCallback : public physx::PxErrorCallback
    {
        virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
    };

    PhysicsEngine();

    physx::PxFoundation* pxFoundation;
    physx::PxPhysics* pxPhysics;
    physx::PxScene* pxScene;
    physx::PxDefaultCpuDispatcher* pxCpuDispatcher;

    AllocatorCallback allocatorCallback;
    ErrorCallback errorCallback;
};

#define sPhysicsEngine PhysicsEngine::getInstance()
