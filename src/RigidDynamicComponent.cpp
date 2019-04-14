#include "RigidDynamicComponent.h"
#include "GameObject.h"

RigidDynamicComponent::RigidDynamicComponent(GameObject* owner) :
    RigidComponent::RigidComponent(owner)
{
}

RigidDynamicComponent::~RigidDynamicComponent()
{
}
