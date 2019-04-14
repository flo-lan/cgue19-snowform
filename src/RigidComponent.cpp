#include "RigidComponent.h"
#include "GameObject.h"

RigidComponent::RigidComponent(GameObject* owner) :
    Component::Component(owner)
{
}

RigidComponent::~RigidComponent()
{
}
