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
#include "pvd/PxPvd.h"
#include "pvd/PxPvdTransport.h"
#include "pvd/PxPvdSceneClient.h"
#include "PxPhysics.h"
#include "PxPhysicsVersion.h"
#include "PxMaterial.h"
#include "PxScene.h"
#include "PxSceneDesc.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "GameObject.h"
#include "Collision.h"
#include "Time.h"
#include "Mesh.h"
#include <iostream>

void PhysicsEngine::ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    fprintf(stderr, "PhysX Error %i: %s! File: %s, Line: %i\n", ((int)code), message, file, line);
}

PhysicsEngine::PhysicsEngine() :
    pxFoundation(nullptr),
    pxPvd(nullptr),
    pxPhysics(nullptr),
    pxCooking(nullptr),
    pxScene(nullptr),
    pxCpuDispatcher(nullptr)
{
}

physx::PxFilterFlags SimulationFilterShader(
    physx::PxFilterObjectAttributes /*attributes0*/, physx::PxFilterData /*filterData0*/,
    physx::PxFilterObjectAttributes /*attributes1*/, physx::PxFilterData /*filterData1*/,
    physx::PxPairFlags& retPairFlags, const void* /*constantBlock*/, physx::PxU32 /*constantBlockSize*/)
{
    retPairFlags = physx::PxPairFlag::eSOLVE_CONTACT |
        physx::PxPairFlag::eDETECT_DISCRETE_CONTACT |
        physx::PxPairFlag::eNOTIFY_TOUCH_FOUND |
        physx::PxPairFlag::eNOTIFY_TOUCH_LOST |
        physx::PxPairFlag::eDETECT_CCD_CONTACT;

    return physx::PxFilterFlag::eDEFAULT;
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

#ifdef _DEBUG
    pxPvd = physx::PxCreatePvd(*pxFoundation);

    if (!pxPvd)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX pvd!\n");
        return false;
    }

    physx::PxPvdTransport* pxPvdTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

    if (pxPvdTransport)
    {
        pxPvd->connect(*pxPvdTransport, physx::PxPvdInstrumentationFlag::eALL);
    }
#endif

    pxPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *pxFoundation, physx::PxTolerancesScale(), true, pxPvd);

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
    pxSceneDesc.filterShader = SimulationFilterShader;
    pxSceneDesc.flags = physx::PxSceneFlags
    (
        physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS |
        physx::PxSceneFlag::eEXCLUDE_KINEMATICS_FROM_ACTIVE_ACTORS |
        physx::PxSceneFlag::eENABLE_CCD
    );
    pxSceneDesc.simulationEventCallback = this;
    pxScene = pxPhysics->createScene(pxSceneDesc);

    if (!pxScene)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX scene!\n");
        return false;
    }

#ifdef _DEBUG
    physx::PxPvdSceneClient* pxPvdSceneClient = pxScene->getScenePvdClient();

    if (pxPvdSceneClient)
    {
        pxPvdSceneClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pxPvdSceneClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pxPvdSceneClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
#endif

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

    if (pxPvd)
    {
        pxPvd->release();
        pxPvd = nullptr;
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

void PhysicsEngine::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
    // Ignore contact when actors have been removed
    if (pairHeader.flags & (physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
    {
        return;
    }

    physx::PxActor* pxActor0 = pairHeader.actors[0];
    physx::PxActor* pxActor1 = pairHeader.actors[1];

    RigidComponent* rigidComponent0 = static_cast<RigidComponent*>(pxActor0->userData);
    RigidComponent* rigidComponent1 = static_cast<RigidComponent*>(pxActor1->userData);

    GameObject* gameObject0 = rigidComponent0->GetOwner();
    GameObject* gameObject1 = rigidComponent1->GetOwner();

    for (physx::PxU32 i = 0; i < nbPairs; i++)
    {
        physx::PxContactPair const& pxContactPair = pairs[i];

        // Ignore pair when shapes have been deleted
        if (pxContactPair.flags & (physx::PxContactPairFlag::eREMOVED_SHAPE_0 | physx::PxContactPairFlag::eREMOVED_SHAPE_1))
        {
            continue;
        }

        ColliderComponent* hitCollider0 = rigidComponent1->GetAttachedColliderByPxShape(pxContactPair.shapes[0]);

        if (!hitCollider0)
        {
            hitCollider0 = rigidComponent1->GetAttachedColliderByPxShape(pxContactPair.shapes[1]);
        }

        ColliderComponent* hitCollider1 = rigidComponent0->GetAttachedColliderByPxShape(pxContactPair.shapes[0]);

        if (!hitCollider1)
        {
            hitCollider1 = rigidComponent0->GetAttachedColliderByPxShape(pxContactPair.shapes[1]);
        }

        Collision collision0 = Collision(hitCollider0, rigidComponent1);
        Collision collision1 = Collision(hitCollider1, rigidComponent0);

        if (pxContactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            gameObject0->CollisionEnter(collision0);
            gameObject1->CollisionEnter(collision1);
        }
        else if (pxContactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            gameObject0->CollisionExit(collision0);
            gameObject1->CollisionExit(collision1);
        }
    }
}

void PhysicsEngine::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    for (physx::PxU32 i = 0; i < count; i++)
    {
        physx::PxTriggerPair const& pxTriggerPair = pairs[i];

        // Ignore pair when shapes have been deleted
        if (pxTriggerPair.flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
        {
            continue;
        }

        RigidComponent* triggerRigidComponent = static_cast<RigidComponent*>(pxTriggerPair.triggerActor->userData);
        RigidComponent* otherRigidComponent = static_cast<RigidComponent*>(pxTriggerPair.otherActor->userData);

        ColliderComponent* triggerCollider = triggerRigidComponent->GetAttachedColliderByPxShape(pxTriggerPair.triggerShape);
        ColliderComponent* otherCollider = otherRigidComponent->GetAttachedColliderByPxShape(pxTriggerPair.otherShape);

        GameObject* trigger = triggerRigidComponent->GetOwner();
        GameObject* other = otherRigidComponent->GetOwner();

        if (pxTriggerPair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            trigger->TriggerEnter(otherCollider);
            other->TriggerEnter(triggerCollider);
        }
        else if (pxTriggerPair.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            trigger->TriggerExit(otherCollider);
            other->TriggerExit(triggerCollider);
        }
    }
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

    pxTriangleMeshDesc.triangles.count = indices.size() / 3;
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
