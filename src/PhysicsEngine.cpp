#include "PhysicsEngine.h"
#include "RigidComponent.h"
#include "common/PxTolerancesScale.h"
#include "cooking/PxCooking.h"
#include "cooking/PxTriangleMeshDesc.h"
#include "extensions/PxExtensionsAPI.h"
#include "foundation/PxTransform.h"
#include "geometry/PxTriangleMesh.h"
#include "geometry/PxGeometry.h"
#include "PxFoundation.h"
#include "PxPhysics.h"
#include "PxPhysicsVersion.h"
#include "PxMaterial.h"
#include "PxScene.h"
#include "PxSceneDesc.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "Time.h"
#include "Mesh.h"
#include <iostream>

void PhysicsEngine::ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    fprintf(stderr, "PhysX Error %i: %s! File: %s, Line: %i\n", ((int)code), message, file, line);
}

PhysicsEngine::PhysicsEngine() :
    pxFoundation(nullptr),
    pxPhysics(nullptr),
    pxCooking(nullptr),
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

    if (!PxInitExtensions(*pxPhysics, nullptr))
    {
        fprintf(stderr, "PhysX Error: Could not initialize PhysX extensions!\n");
        return false;
    }

    pxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pxFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));

    if (!pxCooking)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX cooking!\n");
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
    if (!pxScene)
    {
        return;
    }

    pxScene->simulate(sTime.GetDeltaTime());
    pxScene->fetchResults(true /* block */);

    physx::PxU32 pxActiveActorCount = 0;
    physx::PxActor** pxActiveActors = pxScene->getActiveActors(pxActiveActorCount);

    for (physx::PxU32 i = 0; i < pxActiveActorCount; ++i)
    {
        physx::PxRigidActor* pxRigidActor = static_cast<physx::PxRigidActor*>(pxActiveActors[i]);

        if (RigidComponent* rigidComponent = static_cast<RigidComponent*>(pxRigidActor->userData))
        {
            rigidComponent->SetTransform(pxRigidActor->getGlobalPose());
        }
    }
}

void PhysicsEngine::Stop()
{
    DeletePxMaterials();
    DeletePxTriangleMeshes();

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

    if (pxCooking)
    {
        pxCooking->release();
        pxCooking = nullptr;
    }

    PxCloseExtensions();

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

void PhysicsEngine::InsertRigidActor(physx::PxRigidActor* pxRigidActor)
{
    if (!pxRigidActor)
    {
        return;
    }

    if (!pxScene)
    {
        fprintf(stderr, "PhysX Error: Could not insert PhysX rigid actor, because PhysX scene is null!\n");
        return;
    }

    pxScene->addActor(*pxRigidActor);
}

void PhysicsEngine::RemoveRigidActor(physx::PxRigidActor* pxRigidActor)
{
    if (!pxRigidActor)
    {
        return;
    }

    if (!pxScene)
    {
        fprintf(stderr, "PhysX Error: Could not insert PhysX rigid actor, because PhysX scene is null!\n");
        return;
    }

    pxScene->removeActor(*pxRigidActor);
}

physx::PxMaterial* PhysicsEngine::CreatePxMaterial(std::string const& name,
    float staticFriction, float dynamicFriction, float restitution)
{
    if (!pxPhysics)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX material, because PhysX physics is null!\n");
        return nullptr;
    }

    PxMaterialMap::const_iterator itr = pxMaterials.find(name);

    if (itr != pxMaterials.end())
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX material, because the name '%s' is already used!\n", name.c_str());
        return nullptr;
    }

    physx::PxMaterial* pxMaterial = pxPhysics->createMaterial(staticFriction, dynamicFriction, restitution);

    if (pxMaterial)
    {
        pxMaterials[name] = pxMaterial;
    }

    return pxMaterial;
}

physx::PxTriangleMesh* PhysicsEngine::CreatePxTriangleMesh(std::string const& name,
    std::vector<Vertex> const& vertices, std::vector<uint32_t> const& indices)
{
    if (!pxCooking)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX triangle mesh, because PhysX cooking is null!\n");
        return nullptr;
    }

    PxTriangleMeshMap::const_iterator itr = pxTriangleMeshes.find(name);

    if (itr != pxTriangleMeshes.end())
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX triangle mesh, because the name '%s' is already used!\n", name.c_str());
        return nullptr;
    }

    physx::PxTriangleMeshDesc pxTriangleMeshDesc;

    pxTriangleMeshDesc.points.count = vertices.size();
    pxTriangleMeshDesc.points.stride = sizeof(Vertex);
    pxTriangleMeshDesc.points.data = vertices.data();

    pxTriangleMeshDesc.triangles.count = indices.size();
    pxTriangleMeshDesc.triangles.stride = 3 * sizeof(uint32_t);
    pxTriangleMeshDesc.triangles.data = indices.data();

    physx::PxTriangleMesh* pxTriangleMesh = pxCooking->createTriangleMesh(pxTriangleMeshDesc,
        pxPhysics->getPhysicsInsertionCallback());

    if (pxTriangleMesh)
    {
        pxTriangleMeshes[name] = pxTriangleMesh;
    }

    return pxTriangleMesh;
}

physx::PxShape* PhysicsEngine::CreatePxShape(const physx::PxGeometry& geometry, const physx::PxMaterial& material)
{
    if (!pxPhysics)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX shape, because PhysX physics is null!\n");
        return nullptr;
    }

    return pxPhysics->createShape(geometry, material);
}

physx::PxRigidDynamic* PhysicsEngine::CreatePxRigidDynamic(const physx::PxTransform& pose)
{
    if (!pxPhysics)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX rigid dynamic, because PhysX physics is null!\n");
        return nullptr;
    }

    return pxPhysics->createRigidDynamic(pose);
}

physx::PxRigidStatic* PhysicsEngine::CreatePxRigidStatic(const physx::PxTransform& pose)
{
    if (!pxPhysics)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX rigid static, because PhysX physics is null!\n");
        return nullptr;
    }

    return pxPhysics->createRigidStatic(pose);
}

physx::PxMaterial* PhysicsEngine::GetPxMaterial(std::string const& name)
{
    PxMaterialMap::const_iterator itr = pxMaterials.find(name);
    return itr != pxMaterials.end() ? itr->second : nullptr;
}

physx::PxTriangleMesh* PhysicsEngine::GetPxTriangleMesh(std::string const& name)
{
    PxTriangleMeshMap::const_iterator itr = pxTriangleMeshes.find(name);
    return itr != pxTriangleMeshes.end() ? itr->second : nullptr;
}

void PhysicsEngine::DeletePxMaterials()
{
    for (PxMaterialMap::const_iterator itr = pxMaterials.begin(); itr != pxMaterials.end(); ++itr)
    {
        itr->second->release();
    }

    pxMaterials.clear();
}

void PhysicsEngine::DeletePxTriangleMeshes()
{
    for (PxTriangleMeshMap::const_iterator itr = pxTriangleMeshes.begin(); itr != pxTriangleMeshes.end(); ++itr)
    {
        itr->second->release();
    }

    pxTriangleMeshes.clear();
}
