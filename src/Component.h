#pragma once

#include <vector>

class GameObject;

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

    virtual void Update() {}
    virtual void LateUpdate() {}
    virtual void Render() {}

    virtual void OnDestroy() {}

    void Destroy() { destroyed = true; OnDestroy(); }

    GameObject* GetOwner() const { return owner; }
    bool IsDestroyed() const { return destroyed; }

private:
    GameObject* owner;
    uint32_t typeId;
    bool destroyed;
};
