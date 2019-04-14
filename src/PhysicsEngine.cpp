#include "PhysicsEngine.h"
#include "PxFoundation.h"
#include "PxPhysics.h"
#include "PxPhysicsVersion.h"
#include "PxScene.h"
#include "PxSceneDesc.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "common/PxTolerancesScale.h"
#include "Time.h"
#include <iostream>

void PhysicsEngine::ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    fprintf(stderr, "PhysX Error %i: %s! File: %s, Line: %i\n", ((int)code), message, file, line);
}

PhysicsEngine::PhysicsEngine() :
    pxFoundation(nullptr),
    pxPhysics(nullptr),
    pxScene(nullptr),
    pxCpuDispatcher(nullptr)
{
}

bool PhysicsEngine::Start()
{
    if (pxFoundation)
    {
        fprintf(stderr, "PhysX Error: PhysX foundation was already created!\n");
        return false;
    }

    pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCallback, errorCallback);

    if (!pxFoundation)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX foundation!\n");
        return false;
    }

    pxPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *pxFoundation, physx::PxTolerancesScale(), true);

    if (!pxPhysics)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX base physics!\n");
        return false;
    }

    pxCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);

    if (!pxCpuDispatcher)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX cpu dispatcher!\n");
        return false;
    }

    physx::PxSceneDesc pxSceneDesc(pxPhysics->getTolerancesScale());
    pxSceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    pxSceneDesc.cpuDispatcher = pxCpuDispatcher;
    pxSceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    pxScene = pxPhysics->createScene(pxSceneDesc);

    if (!pxScene)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX scene!\n");
        return false;
    }

    return true;
}

void PhysicsEngine::Update()
{
    if (pxScene)
    {
        pxScene->simulate(sTime.GetDeltaTime());
        pxScene->fetchResults(true);
    }
}

void PhysicsEngine::Stop()
{
    if (pxScene)
    {
        pxScene->release();
        pxScene = nullptr;
    }

    if (pxCpuDispatcher)
    {
        pxCpuDispatcher->release();
        pxCpuDispatcher = nullptr;
    }

    if (pxPhysics)
    {
        pxPhysics->release();
        pxPhysics = nullptr;
    }

    if (pxFoundation)
    {
        pxFoundation->release();
        pxFoundation = nullptr;
    }
}
