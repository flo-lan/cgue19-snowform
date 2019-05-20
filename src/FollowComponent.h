#pragma once

#include "Component.h"
#include <string>

class TransformComponent;

class FollowComponent : public Component
{
public:
    FollowComponent(GameObject* owner);

    virtual void Update();

    void SetTargetId(std::string const& targetId) { this->targetId = targetId; }

private:
    TransformComponent* transform;
    std::string targetId;
};
