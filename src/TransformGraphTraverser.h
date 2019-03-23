#pragma once

class TransformComponent;

struct TransformGraphTraverser
{
    virtual ~TransformGraphTraverser() {}

    virtual void Visit(TransformComponent* transform) = 0;
};
