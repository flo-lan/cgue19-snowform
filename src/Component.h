#pragma once

class GameObject;

class Component
{
public:
    Component(GameObject* owner);
    virtual ~Component();

    virtual void OnAwake() {}
    virtual void OnStart() {}

    virtual void Update() {}
    virtual void LateUpdate() {}
    virtual void Render() {}

    virtual void OnDestroy() {}

    void Destroy() { destroyed = true; OnDestroy(); }

    GameObject* GetOwner() const { return owner; }
    bool IsDestroyed() const { return destroyed; }

private:
    GameObject* owner;
    bool destroyed;
};
