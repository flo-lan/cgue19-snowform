#pragma once

#include "Component.h"

class DebugComponent : public Component
{
public:
    DebugComponent(GameObject* owner);
    virtual ~DebugComponent();

    virtual void OnAwake();
    virtual void OnStart();

    virtual void OnAttachComponent(Component* component);
    virtual void OnRemoveComponent(Component* component);

    virtual void OnCollisionEnter(Collision const& collision);
    virtual void OnCollisionExit(Collision const& collision);

    virtual void OnTriggerEnter(ColliderComponent* other);
    virtual void OnTriggerExit(ColliderComponent* other);

    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();

    virtual void OnDestroy();
};
