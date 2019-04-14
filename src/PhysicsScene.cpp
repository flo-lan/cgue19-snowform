#include "PhysicsScene.h"
#include "PxFoundation.h"
#include "PxPhysicsVersion.h"
#include "Scene.h"
#include <iostream>

void PhysicsScene::ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    fprintf(stderr, "PhysX Error %i: %s! File: %s, Line: %i\n", ((int)code), message, file, line);
}

PhysicsScene::PhysicsScene(Scene* _ownerScene) :
    ownerScene(_ownerScene),
    foundation(nullptr)
{
    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCallback, errorCallback);

    if (!foundation)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX foundation!\n");
    }
}

PhysicsScene::~PhysicsScene()
{
    if (foundation)
    {
        foundation->release();
        foundation = nullptr;
    }
}
