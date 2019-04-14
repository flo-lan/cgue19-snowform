#include "PhysicsEngine.h"
#include "PxFoundation.h"
#include "PxPhysicsVersion.h"
#include <iostream>

void PhysicsEngine::ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    fprintf(stderr, "PhysX Error %i: %s! File: %s, Line: %i\n", ((int)code), message, file, line);
}

PhysicsEngine::PhysicsEngine() :
    foundation(nullptr)
{
}

bool PhysicsEngine::Start()
{
    if (foundation)
    {
        fprintf(stderr, "PhysX Error: PhysX foundation was already created!\n");
        return false;
    }

    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCallback, errorCallback);

    if (!foundation)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX foundation!\n");
        return false;
    }

    return true;
}

void PhysicsEngine::Stop()
{
    if (foundation)
    {
        foundation->release();
        foundation = nullptr;
    }
}
