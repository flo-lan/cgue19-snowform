#pragma once
#include "Component.h"
#include <string>

class TextRendererComponent : public Component
{
public:
    TextRendererComponent(GameObject* owner);
    void RebuildTextMesh(std::string text);
    //virtual ~TextRendererComponent();

    void Render() override;
};
