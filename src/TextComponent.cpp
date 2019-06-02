#include "TextComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "GameObject.h"
#include "GlyphBlock.h"
#include "Glyph.h"
#include "Material.h"
#include "Mesh.h"

TextComponent::TextComponent(GameObject* owner) :
    Component::Component(owner),
    transform(owner->GetComponent<TransformComponent>()),
    meshRenderer(owner->AttachComponent<MeshRendererComponent>()),
    textAlignment(TEXT_ALIGNMENT_TOP_LEFT),
    glyphBlock(new GlyphBlock()),
    textMesh(new Mesh("Dynamic Textmesh")),
    isDirty(false)
{
    fprintf(stdout, "Attached text component to game object '%s'!\n", GetOwner()->GetName().c_str());
}

TextComponent::~TextComponent()
{
    fprintf(stdout, "Deleted text component from game object '%s'!\n", GetOwner()->GetName().c_str());

    delete textMesh;
    delete glyphBlock;
}

void TextComponent::OnRemoveComponent(Component* component)
{
    if (component == meshRenderer)
    {
        meshRenderer = nullptr;
    }
}

void TextComponent::OnDestroy()
{
    if (meshRenderer)
    {
        meshRenderer->Destroy();
    }
}

void TextComponent::LateUpdate()
{
    if (isDirty)
    {
        isDirty = false;

        glyphBlock->UpdateGlyphLines();

        RebuildTextMesh();
    }
}

void TextComponent::SetEnabled(bool enable)
{
    if (meshRenderer)
    {
        meshRenderer->SetEnabled(enable);
    }
}

void TextComponent::SetMaterial(Material* material)
{
    if (!meshRenderer)
    {
        return;
    }

    meshRenderer->SetMaterial(material);
}

void TextComponent::SetFontSize(float fontSize)
{
    glyphBlock->SetFontSize(fontSize);

    transform->SetLocalScale(fontSize, fontSize, 0.f);

    isDirty = true;
}

Material* TextComponent::GetMaterial() const
{
    return meshRenderer ? meshRenderer->GetMaterial() : nullptr;
}

void TextComponent::RebuildTextMesh()
{
    if (!meshRenderer)
    {
        return;
    }

    Mesh& mesh = *this->textMesh;

    mesh.Vertices.clear();
    mesh.Indices.clear();

    float maxWidthNorm = glyphBlock->GetMaxWidthNorm();
    float maxHeightNorm = glyphBlock->GetMaxHeightNorm();

    float halfMaxWidthNorm = maxWidthNorm / 2.f;
    float halfMaxHeightNorm = maxHeightNorm / 2.f;

    float widthNorm = glyphBlock->GetWidthNorm();
    float heightNorm = glyphBlock->GetHeightNorm();

    float halfWidthNorm = widthNorm / 2.f;
    float halfHeightNorm = heightNorm / 2.f;

    float glyphPositionX = 0.f;
    float glyphPositionY = 0.f;
    float glyphLineOffsetY = 0.f;

    int startIndex = 0;

    GlyphLineList const& glyphLines = glyphBlock->GetGlyphLines();

    for (auto itr = glyphLines.begin(); itr != glyphLines.end(); ++itr)
    {
        GlyphLine* glyphLine = *itr;

        float lineWidthNorm = glyphLine->WidthNorm;
        float lineHeightNorm = glyphLine->HeightNorm;

        float halfLineWidthNorm = lineWidthNorm / 2.f;
        float halfLineHeightNorm = lineHeightNorm / 2.f;

        switch (textAlignment)
        {
            case TEXT_ALIGNMENT_TOP_LEFT:
                glyphPositionX = 0.f;
                glyphPositionY = 0.f - glyphLineOffsetY;
                break;
            case TEXT_ALIGNMENT_TOP_CENTER:
                glyphPositionX = halfMaxWidthNorm - halfLineWidthNorm;
                glyphPositionY = 0.f - glyphLineOffsetY;
                break;
            case TEXT_ALIGNMENT_TOP_RIGHT:
                glyphPositionX = maxWidthNorm - lineWidthNorm;
                glyphPositionY = 0.f - glyphLineOffsetY;
                break;
            case TEXT_ALIGNMENT_CENTER_LEFT:
                glyphPositionX = 0.f;
                glyphPositionY = -(halfMaxHeightNorm - halfHeightNorm) - glyphLineOffsetY;
                break;
            case TEXT_ALIGNMENT_CENTER_CENTER:
                glyphPositionX = halfMaxWidthNorm - halfLineWidthNorm;
                glyphPositionY = -(halfMaxHeightNorm - halfHeightNorm) - glyphLineOffsetY;
                break;
            case TEXT_ALIGNMENT_CENTER_RIGHT:
                glyphPositionX = maxWidthNorm - lineWidthNorm;
                glyphPositionY = -(halfMaxHeightNorm - halfHeightNorm) - glyphLineOffsetY;
                break;
            case TEXT_ALIGNMENT_BOTTOM_LEFT:
                glyphPositionX = 0.f;
                glyphPositionY = -(maxHeightNorm - heightNorm) - glyphLineOffsetY;
                break;
            case TEXT_ALIGNMENT_BOTTOM_CENTER:
                glyphPositionX = halfMaxWidthNorm - halfLineWidthNorm;
                glyphPositionY = -(maxHeightNorm - heightNorm) - glyphLineOffsetY;
                break;
            case TEXT_ALIGNMENT_BOTTOM_RIGHT:
                glyphPositionX = maxWidthNorm - lineWidthNorm;
                glyphPositionY = -(maxHeightNorm - heightNorm) - glyphLineOffsetY;
                break;
        }

        GlyphList const& glyphs = glyphLine->Glyphs;

        for (int i = 0, length = (int)glyphs.size(); i < length; i++)
        {
            Glyph const& glyph = *glyphs[i];

            if ((char)glyph.GetId() != ' ')
            {
                mesh.Vertices.push_back(Vertex
                (
                    glyphPositionX + glyph.GetXOffsetNorm(),
                    glyphPositionY - glyph.GetYOffsetNorm() - glyph.GetHNorm(),
                    0.f,
                    0.f, 0.f, 1.f,
                    glyph.GetUV00()
                ));

                mesh.Vertices.push_back(Vertex
                (
                    glyphPositionX + glyph.GetXOffsetNorm(),
                    glyphPositionY - glyph.GetYOffsetNorm(),
                    0.f,
                    0.f, 0.f, 1.f,
                    glyph.GetUV01()
                ));

                mesh.Vertices.push_back(Vertex
                (
                    glyphPositionX + glyph.GetWNorm() + glyph.GetXOffsetNorm(),
                    glyphPositionY - glyph.GetHNorm() - glyph.GetYOffsetNorm(),
                    0.f,
                    0.f, 0.f, 1.f,
                    glyph.GetUV10()
                ));

                mesh.Vertices.push_back(Vertex
                (
                    glyphPositionX + glyph.GetWNorm() + glyph.GetXOffsetNorm(),
                    glyphPositionY - glyph.GetYOffsetNorm(),
                    0.f,
                    0.f, 0.f, 1.f,
                    glyph.GetUV11()
                ));

                mesh.Indices.push_back(startIndex + 0);  mesh.Indices.push_back(startIndex + 2);  mesh.Indices.push_back(startIndex + 1);
                mesh.Indices.push_back(startIndex + 2);  mesh.Indices.push_back(startIndex + 3);  mesh.Indices.push_back(startIndex + 1);

                startIndex += 4;
            }

            glyphPositionX += glyph.GetXAdvanceNorm();
        }

        glyphLineOffsetY += lineHeightNorm;
    }

    if (mesh.Vertices.size())
    {
        textMesh->Upload(); // Hack to update static meshes; ToDo: Implement dynamic mesh drawing for better performance
        meshRenderer->SetMesh(textMesh);
    }
    else
    {
        meshRenderer->SetMesh(nullptr);
    }

    //fprintf(stdout, "Rebuilt text mesh for text '%s' with %i quads and %i indices!\n",
    //        glyphBlock->GetText().c_str(), (int)(mesh.Vertices.size() / 4), (int)mesh.Indices.size());
}
