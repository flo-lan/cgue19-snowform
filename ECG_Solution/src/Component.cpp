#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* _owner) :
    owner(_owner),
    destroyed(false)
{
    OnAwake();
}

Component::~Component()
{
}
