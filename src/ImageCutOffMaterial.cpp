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
        shaderProgram->SetUniformMatrix4fv(0, camera->GetOrthographicProjectionMatrix());
        shaderProgram->SetUniformMatrix4fv(1, renderer->GetTransform()->GetModelMatrix());

        if (defaultTexture)
        {
            defaultTexture->ActivateAndBind(0 /* Unit */);
        }

        if (cutOffTexture)
        {
            shaderProgram->SetUniform1i(2, 1 /* Unit */);

            cutOffTexture->ActivateAndBind(1 /* Unit */);
        }
        else
        {
            shaderProgram->SetUniform1i(2, 0 /* Unit */);
        }

        if (imageTexture)
        {
            shaderProgram->SetUniform1i(3, 2 /* Unit */);

            imageTexture->ActivateAndBind(2 /* Unit */);
        }
        else
        {
            shaderProgram->SetUniform1i(3, 0 /* Unit */);
        }

        shaderProgram->SetUniform4fv(4, imageColor);
        shaderProgram->SetUniform1fv(5, cutOff);
    }
}
