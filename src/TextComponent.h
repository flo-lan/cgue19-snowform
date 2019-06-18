#pragma once

#include "Component.h"
#include "GlyphBlock.h"
#include <glm/glm.hpp>
#include <string>

class GlyphBlock;
class Material;
class Mesh;
class MeshRendererComponent;
class TransformComponent;

enum TextAlignment
{
    TEXT_ALIGNMENT_TOP_LEFT,
    TEXT_ALIGNMENT_TOP_CENTER,
    TEXT_ALIGNMENT_TOP_RIGHT,
    TEXT_ALIGNMENT_CENTER_LEFT,
    TEXT_ALIGNMENT_CENTER_CENTER,
    TEXT_ALIGNMENT_CENTER_RIGHT,
    TEXT_ALIGNMENT_BOTTOM_LEFT,
    TEXT_ALIGNMENT_BOTTOM_CENTER,
    TEXT_ALIGNMENT_BOTTOM_RIGHT
};

class TextComponent : public Component
{
public:
    TextComponent(GameObject* owner);
    virtual ~TextComponent();

    virtual void OnRemoveComponent(Component* component);
    virtual void OnDestroy();

    virtual void LateUpdate();

    void SetEnabled(bool enable);
    void SetMaterial(Material* material);
    void SetFont(Font* font) { glyphBlock->SetFont(font); isDirty = isOutlineDirty = true; }
    void SetFontSize(float fontSize);
    void SetText(std::string const& text) { glyphBlock->SetText(text); isDirty = isOutlineDirty = true; }
    void SetTextAlignment(TextAlignment textAlignment) { this->textAlignment = textAlignment; isDirty = isOutlineDirty = true; }
    void SetTextOverflowMode(TextOverflowMode textOverflowMode) { glyphBlock->SetTextOverflowMode(textOverflowMode); isDirty = isOutlineDirty = true; }
    void SetWordWrapping(bool enable) { glyphBlock->SetWordWrapping(enable); isDirty = isOutlineDirty = true; }
    void SetMaxWidth(float maxWidth) { glyphBlock->SetMaxWidth(maxWidth); isDirty = isOutlineDirty = true; }
    void SetMaxHeight(float maxHeight) { glyphBlock->SetMaxHeight(maxHeight); isDirty = isOutlineDirty = true; }
    void SetOutline(bool outline);
    void SetOutlineMaterial(Material* outlineMaterial);
    void SetOutlineThickness(float outlineThickness);
    void SetOutlineOffset(glm::vec3 outlineOffset);

    Material* GetMaterial() const;
    Font* GetFont() const { return glyphBlock->GetFont(); }
    float GetFontSize() const { return glyphBlock->GetFontSize(); }
    std::string const& GetText() const { return glyphBlock->GetText(); }
    TextAlignment GetTextAlignment() const { return textAlignment; }
    TextOverflowMode GetTextOverflowMode() const { return glyphBlock->GetTextOverflowMode(); }
    bool IsWordWrappingEnabled() const { return glyphBlock->IsWordWrappingEnabled(); }
    float GetMaxWidth() const { return glyphBlock->GetMaxWidth(); }
    float GetMaxHeight() const { return glyphBlock->GetMaxHeight(); }
    float GetWidth() const { return glyphBlock->GetWidth(); }
    float GetHeight() const { return glyphBlock->GetHeight(); }

private:
    void RebuildTextMesh();
    void RebuildOutlineMesh();

    TransformComponent* transform;
    MeshRendererComponent* meshRenderer;
    MeshRendererComponent* meshRendererOutline;
    TextAlignment textAlignment;
    GlyphBlock* glyphBlock;
    Mesh* textMesh;
    Mesh* textMeshOutline;
    bool outline;
    float outlineThickness;
    glm::vec3 outlineOffset;
    bool isOutlineDirty;
    bool isDirty;
};
