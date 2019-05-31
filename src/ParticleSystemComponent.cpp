#include "ParticleSystemComponent.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* owner) :
    Component::Component(owner)
{
    fprintf(stdout, "Attached mesh renderer component to game object '%s'!\n", GetOwner()->GetName().c_str());
}

ParticleSystemComponent::~ParticleSystemComponent()
{
}
