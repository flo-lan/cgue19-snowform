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
    meshRenderer(nullptr),
    meshRendererOutline(nullptr),
    textAlignment(TEXT_ALIGNMENT_TOP_LEFT),
    glyphBlock(new GlyphBlock()),
    textMesh(new Mesh("Dynamic Textmesh")),
    textMeshOutline(new Mesh("Dynamic Textmesh Outline")),
    outline(false),
    outlineThickness(1.f),
    outlineOffset(0.f),
    isOutlineDirty(false),
    isDirty(false)
{
    fprintf(stdout, "Attached text component to game object '%s'!\n", GetOwner()->GetName().c_str());

    // Attach outline renderer first (!) Render order is important
    meshRendererOutline = owner->AttachComponent<MeshRendererComponent>();
    meshRenderer = owner->AttachComponent<MeshRendererComponent>();

    // Disable view frustum culling for text meshes, because text bounds are NYI
    meshRendererOutline->SetDontCull(true);
    meshRenderer->SetDontCull(true);
}

TextComponent::~TextComponent()
{
    fprintf(stdout, "Deleted text component from game object '%s'!\n", GetOwner()->GetName().c_str());

    delete textMeshOutline;
    delete textMesh;
    delete glyphBlock;
}

void TextComponent::OnRemoveComponent(Component* component)
{
    if (component == meshRenderer)
    {
        meshRenderer = nullptr;
    }
    else if (component == meshRendererOutline)
    {
        meshRendererOutline = nullptr;
    }
}

void TextComponent::OnDestroy()
{
    if (meshRenderer)
    {
        meshRenderer->Destroy();
    }

    if (meshRendererOutline)
    {
        meshRendererOutline->Destroy();
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

    if (isOutlineDirty && outline)
    {
        isOutlineDirty = false;

        RebuildOutlineMesh();
    }
}

void TextComponent::SetEnabled(bool enable)
{
    if (meshRenderer)
    {
        meshRenderer->SetEnabled(enable);
    }

    if (meshRendererOutline && outline)
    {
        meshRendererOutline->SetEnabled(enable);
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
    isOutlineDirty = true;
}

void TextComponent::SetOutline(bool outline)
{
    if (!this->outline && outline)
    {
        isOutlineDirty = true;
    }

    this->outline = outline;

    if (meshRendererOutline)
    {
        meshRendererOutline->SetEnabled(meshRenderer && meshRenderer->IsEnabled());
    }
}

void TextComponent::SetOutlineMaterial(Material* outlineMaterial)
{
    if (!meshRendererOutline)
    {
        return;
    }

    meshRendererOutline->SetMaterial(outlineMaterial);
}

void TextComponent::SetOutlineThickness(float outlineThickness)
{
    if (this->outlineThickness != outlineThickness)
    {
        isOutlineDirty = true;
    }

    this->outlineThickness = outlineThickness;
}

void TextComponent::SetOutlineOffset(glm::vec3 outlineOffset)
{
    if (this->outlineOffset != outlineOffset)
    {
        isOutlineDirty = true;
    }

    this->outlineOffset = outlineOffset;
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

void TextComponent::RebuildOutlineMesh()
{
    if (!meshRenderer)
    {
        return;
    }

    if (!meshRendererOutline)
    {
        return;
    }

    Mesh& mesh = *this->textMesh;
    Mesh& outlineMesh = *this->textMeshOutline;

    if (mesh.Vertices.size())
    {
        outlineMesh.Vertices.resize(mesh.Vertices.size());
    }
    else
    {
        outlineMesh.Vertices.clear();
    }

    if (mesh.Indices.size())
    {
        outlineMesh.Indices.resize(mesh.Indices.size());
    }
    else
    {
        outlineMesh.Indices.clear();
    }

    std::copy(mesh.Vertices.begin(), mesh.Vertices.end(), outlineMesh.Vertices.begin());
    std::copy(mesh.Indices.begin(), mesh.Indices.end(), outlineMesh.Indices.begin());

    static const glm::vec3 dir00 = glm::vec3(-1.f, -1.f, 0.f);
    static const glm::vec3 dir01 = glm::vec3(-1.f,  1.f, 0.f);
    static const glm::vec3 dir10 = glm::vec3( 1.f, -1.f, 0.f);
    static const glm::vec3 dir11 = glm::vec3( 1.f,  1.f, 0.f);

    glm::vec3 offset00 = dir00 * outlineThickness + outlineOffset;
    glm::vec3 offset01 = dir01 * outlineThickness + outlineOffset;
    glm::vec3 offset10 = dir10 * outlineThickness + outlineOffset;
    glm::vec3 offset11 = dir11 * outlineThickness + outlineOffset;

    for (size_t i = 0; i < outlineMesh.Vertices.size(); i += 4)
    {
        Vertex& v00 = outlineMesh.Vertices[i];
        Vertex& v01 = outlineMesh.Vertices[i+1];
        Vertex& v10 = outlineMesh.Vertices[i+2];
        Vertex& v11 = outlineMesh.Vertices[i+3];

        v00.Position = glm::vec3(v00.Position) + offset00;
        v01.Position = glm::vec3(v01.Position) + offset01;
        v10.Position = glm::vec3(v10.Position) + offset10;
        v11.Position = glm::vec3(v11.Position) + offset11;
    }

    if (outlineMesh.Vertices.size())
    {
        textMeshOutline->Upload(); // Hack to update static meshes; ToDo: Implement dynamic mesh drawing for better performance
        meshRendererOutline->SetMesh(textMeshOutline);
    }
    else
    {
        meshRendererOutline->SetMesh(nullptr);
    }
}
