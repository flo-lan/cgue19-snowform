#pragma once

class ColliderComponent;
class GameObject;
class RigidComponent;

class Collision
{
public:
    Collision(ColliderComponent* collider, RigidComponent* rigid);

    ColliderComponent* GetCollider() const { return collider; }
    RigidComponent* GetRigid() const { return rigid; }

    // The GameObject whose collider we are colliding with.
    GameObject* GetGameObject() const;

private:
    ColliderComponent* collider; // The Collider we hit
    RigidComponent* rigid; // The Rigid we hit
};
