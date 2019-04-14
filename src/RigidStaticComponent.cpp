#include "RigidStaticComponent.h"
#include "GameObject.h"

RigidStaticComponent::RigidStaticComponent(GameObject* owner) :
    RigidComponent::RigidComponent(owner)
{
}

RigidStaticComponent::~RigidStaticComponent()
{
}
