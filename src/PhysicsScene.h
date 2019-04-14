#pragma once

class Scene;

class PhysicsScene
{
public:
    PhysicsScene(Scene* ownerScene);
    ~PhysicsScene();

    Scene* GetOwnerScene() const { return ownerScene; }

private:
    Scene* ownerScene;
};
