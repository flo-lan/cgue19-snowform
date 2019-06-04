#include "TextMaterial.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "AssetManager.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

TextMaterial::TextMaterial(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    defaultTexture(sAssetManager.GetTexture2D("pixel_diffuse")),
    atlasTexture(nullptr),
    textColor(1.f, 1.f, 1.f, 1.f)
{
}

TextMaterial::~TextMaterial()
{
}

void TextMaterial::SetUniforms(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(0, camera->GetOrthographicProjectionMatrix());
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
