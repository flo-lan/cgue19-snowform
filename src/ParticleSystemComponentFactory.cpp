#include "ParticleSystemComponentFactory.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"
#include "tinyxml2.h"

ParticleSystemComponentFactory::ParticleSystemComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void ParticleSystemComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    gameObject->AttachComponent<ParticleSystemComponent>();
}
