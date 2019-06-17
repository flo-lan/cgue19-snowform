#include "TextMaterial3D.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "AssetManager.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

TextMaterial3D::TextMaterial3D(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    defaultTexture(sAssetManager.GetTexture2D("pixel_diffuse")),
    atlasTexture(nullptr),
    textColor(1.f, 1.f, 1.f, 1.f)
{
}

TextMaterial3D::~TextMaterial3D()
{
}

void TextMaterial3D::PreRender(CameraComponent* camera, MeshRendererComponent* renderer)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(0, camera->GetViewProjectionMatrix());
        shaderProgram->SetUniformMatrix4fv(1, renderer->GetTransform()->GetModelMatrix());

        if (defaultTexture)
        {
            defaultTexture->ActivateAndBind(0 /* Unit */);
        }

        if (atlasTexture)
        {
            shaderProgram->SetUniform1i(2, 1 /* Unit */);

            atlasTexture->ActivateAndBind(1 /* Unit */);
        }
        else
        {
            shaderProgram->SetUniform1i(2, 0 /* Unit */);
        }

        shaderProgram->SetUniform4fv(3, textColor);
    }
}

void TextMaterial3D::PostRender(CameraComponent * camera, MeshRendererComponent * renderer)
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
