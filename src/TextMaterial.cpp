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

void TextMaterial::SetUniforms(MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(shaderProgram->GetUniformLocation("projection"), renderer->GetCamera() ? renderer->GetCamera()->GetOrthographicProjectionMatrix() : glm::mat4(1.f));
        shaderProgram->SetUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), renderer->GetTransform() ? renderer->GetTransform()->GetModelMatrix() : glm::mat4(1.f));

        if (defaultTexture)
        {
            defaultTexture->ActivateAndBind(0 /* Unit */);
        }

        if (atlasTexture)
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("atlasTexture"), 1 /* Unit */);

            atlasTexture->ActivateAndBind(1 /* Unit */);
        }
        else
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("atlasTexture"), 0 /* Unit */);
        }

        shaderProgram->SetUniform4fv(shaderProgram->GetUniformLocation("textColor"), textColor);
    }
}
