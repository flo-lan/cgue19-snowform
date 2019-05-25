#include "ImageCutOffMaterial.h"
#include "MeshRendererComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

ImageCutOffMaterial::ImageCutOffMaterial(std::string const& name, ShaderProgram* shaderProgram) :
    ImageMaterial::ImageMaterial(name, shaderProgram),
    cutOffTexture(nullptr),
    cutOff(0.f)
{
}

ImageCutOffMaterial::~ImageCutOffMaterial()
{
}

void ImageCutOffMaterial::SetUniforms(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(shaderProgram->GetUniformLocation("projection"), camera->GetOrthographicProjectionMatrix());
        shaderProgram->SetUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), renderer->GetTransform()->GetModelMatrix());

        if (defaultTexture)
        {
            defaultTexture->ActivateAndBind(0 /* Unit */);
        }

        if (cutOffTexture)
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("cutOffTexture"), 1 /* Unit */);

            cutOffTexture->ActivateAndBind(1 /* Unit */);
        }
        else
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("cutOffTexture"), 0 /* Unit */);
        }

        if (imageTexture)
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("imageTexture"), 2 /* Unit */);

            imageTexture->ActivateAndBind(2 /* Unit */);
        }
        else
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("imageTexture"), 0 /* Unit */);
        }

        shaderProgram->SetUniform4fv(shaderProgram->GetUniformLocation("imageColor"), imageColor);
        shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation("cutOff"), cutOff);
    }
}
