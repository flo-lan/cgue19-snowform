#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* _owner) :
    owner(_owner),
    typeId(0),
    destroyed(false)
{
    OnAwake();
}

Component::~Component()
{
}
