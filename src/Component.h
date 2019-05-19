#pragma once

#include <vector>

class GameObject;
class ColliderComponent;
class Collision;

class Component
{
    friend class GameObject;

protected:
    Component(GameObject* owner);
    virtual ~Component();

public:
    virtual void OnAwake() {}
    virtual void OnStart() {}

    virtual void OnAttachComponent(Component* component) {}
    virtual void OnRemoveComponent(Component* component) {}

    // OnCollisionEnter is called when this collider/rigidbody has begun touching another rigidbody/collider.
    virtual void OnCollisionEnter(Collision const& collision) {}
    // OnCollisionExit is called when this collider/rigidbody has stopped touching another rigidbody/collider.
    virtual void OnCollisionExit(Collision const& collision) {}

    // OnTriggerEnter is called when the Collider other enters the trigger.
    virtual void OnTriggerEnter(ColliderComponent* other) {}
    // OnTriggerExit is called when the Collider other has stopped touching the trigger.
    virtual void OnTriggerExit(ColliderComponent* other) {}

    virtual void Update() {}
    virtual void LateUpdate() {}

    virtual void OnDestroy() {}

    void Destroy() { destroyed = true; OnDestroy(); }

    GameObject* GetOwner() const { return owner; }
    uint32_t GetTypeId() const { return typeId;  }
    bool IsDestroyed() const { return destroyed; }

private:
    GameObject* owner;
    uint32_t typeId;
    bool destroyed;
};
