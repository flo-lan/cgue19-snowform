#include "PhysXScene.h"
#include "Scene.h"
#include "Utils.h"

PhysXScene::PhysXScene(std::string const& name) :
    Scene(name)
{
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
        gDefaultErrorCallback);
    if (!mFoundation)
        std::cout << "ERROR: " << "PxCreateFoundation failed!" << std::endl;
}
