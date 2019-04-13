#include "PhysXScene.h"
#include "Scene.h"
#include "Utils.h"
#include "PxFoundation.h"
#include "PxPhysicsVersion.h"

void PhysXScene::ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    fprintf(stderr, "PhysX Error %i: %s! File: %s, Line: %i\n", ((int)code), message, file, line);
}

PhysXScene::PhysXScene(std::string const& name) :
    Scene(name),
    foundation(nullptr)
{
    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocatorCallback, errorCallback);

    if (!foundation)
    {
        fprintf(stderr, "PhysX Error: Could not create PhysX foundation for scene '%s'!\n", name.c_str());
    }
}

PhysXScene::~PhysXScene()
{
    if (foundation)
    {
        foundation->release();
        foundation = nullptr;
    }
}
