#pragma once
#include "Component.h"
#include <string>
#include "TextRendererComponent.h"

class TextComponent : public Component
{
public:
    TextComponent(GameObject* owner);
    //virtual ~TextComponent();

    void Update() override;

    std::string GetText() const { return text; }

    void SetText(const std::string& text);
private:
    std::string text;
    bool isDirty;
    TextRendererComponent* textRenderer;
};
