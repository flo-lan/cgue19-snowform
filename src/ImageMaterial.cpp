#include "ImageMaterial.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "AssetManager.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

ImageMaterial::ImageMaterial(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    defaultTexture(sAssetManager.GetTexture2D("pixel_diffuse")),
    imageTexture(nullptr),
    imageColor(1.f, 1.f, 1.f, 1.f)
{
}

ImageMaterial::~ImageMaterial()
{
}

void ImageMaterial::PreRender(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(0, camera->GetOrthographicProjectionMatrix());
        shaderProgram->SetUniformMatrix4fv(1, renderer->GetTransform()->GetModelMatrix());

        if (defaultTexture)
        {
            defaultTexture->ActivateAndBind(0 /* Unit */);
        }

        if (imageTexture)
        {
            shaderProgram->SetUniform1i(2, 1 /* Unit */);

            imageTexture->ActivateAndBind(1 /* Unit */);
        }
        else
        {
            shaderProgram->SetUniform1i(2, 0 /* Unit */);
        }

        shaderProgram->SetUniform4fv(3, imageColor);
    }
}
