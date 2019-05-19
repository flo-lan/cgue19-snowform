#include "LightComponent.h"
#include "GameObject.h"

LightComponent::LightComponent(GameObject* owner) :
    Component::Component(owner)
{
}

LightComponent::~LightComponent()
{
}