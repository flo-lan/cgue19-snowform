#include "TextComponent.h"
#include "TextRendererComponent.h"
#include "GameObject.h"

TextComponent::TextComponent(GameObject* owner) :
    Component(owner),
    textRenderer(owner->AttachComponent<TextRendererComponent>())
{
   
}

void TextComponent::Update()
{
    if (isDirty)
    {
        // textRenderer->RebuildTextMesh();
        isDirty = false;
    }
}

void TextComponent::SetText(const std::string& text)
{
    if (this->text == text)
    {
        return;
    }
    
    this->text = text;
    isDirty = true;
}
