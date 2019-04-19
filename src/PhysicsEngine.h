#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "foundation/PxErrorCallback.h"
#include "foundation/PxErrors.h"
#include "foundation/Px.h"
#include "PxSimulationEventCallback.h"

#include <string>
#include <unordered_map>
#include <vector>

class PhysicsMaterial;
class RigidComponent;
struct Vertex;

namespace physx
{
    class PxFoundation;
    class PxPvd;
    class PxPhysics;
    class PxCooking;
    class PxScene;
    class PxDefaultCpuDispatcher;
    class PxTriangleMesh;
    class PxShape;
    class PxGeometry;
    class PxMaterial;
    class PxActor;
    class PxRigidActor;
    class PxRigidBody;
    class PxRigidDynamic;
    class PxRigidStatic;
    class PxTransform;
}

class PhysicsEngine : public physx::PxSimulationEventCallback
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

    void InsertRigidActor(physx::PxRigidActor* pxRigidActor);
    void RemoveRigidActor(physx::PxRigidActor* pxRigidActor);

    // Implement PxSimulationEventCallback
    virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
    virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) {}
    virtual void onWake(physx::PxActor**, physx::PxU32) {}
    virtual void onSleep(physx::PxActor**, physx::PxU32) {}
    virtual void onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) {}

    physx::PxMaterial* CreatePxMaterial(std::string const& name,
        float staticFriction, float dynamicFriction, float restitution);
    physx::PxTriangleMesh* CreatePxTriangleMesh(std::string const& name,
        std::vector<Vertex> const& vertices, std::vector<uint32_t> const& indices);
    physx::PxShape* CreatePxShape(const physx::PxGeometry& geometry, const physx::PxMaterial& material);
    physx::PxRigidDynamic* CreatePxRigidDynamic(const physx::PxTransform& pose);
    physx::PxRigidStatic* CreatePxRigidStatic(const physx::PxTransform& pose);

    physx::PxMaterial* GetPxMaterial(std::string const& name);
    physx::PxTriangleMesh* GetPxTriangleMesh(std::string const& name);

    void DeletePxMaterials();
    void DeletePxTriangleMeshes();

private:
    class AllocatorCallback : public physx::PxDefaultAllocator
    {
    };

    class ErrorCallback : public physx::PxErrorCallback
    {
        virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
    };

    typedef std::unordered_map<std::string /* Material Name */, physx::PxMaterial*> PxMaterialMap;
    typedef std::unordered_map<std::string /* Mesh Name */, physx::PxTriangleMesh*> PxTriangleMeshMap;

    PhysicsEngine();

    physx::PxFoundation* pxFoundation;
    physx::PxPvd* pxPvd;
    physx::PxPhysics* pxPhysics;
    physx::PxCooking* pxCooking;
    physx::PxScene* pxScene;
    physx::PxDefaultCpuDispatcher* pxCpuDispatcher;

    PxMaterialMap pxMaterials;
    PxTriangleMeshMap pxTriangleMeshes;

    AllocatorCallback allocatorCallback;
    ErrorCallback errorCallback;
};

#define sPhysicsEngine PhysicsEngine::getInstance()
