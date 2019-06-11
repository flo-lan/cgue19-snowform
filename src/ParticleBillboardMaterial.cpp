#include "ParticleBillboardMaterial.h"
#include "ShaderProgram.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "AssetManager.h"
#include "Texture2D.h"
#include <glm/gtc/matrix_transform.hpp>

ParticleBillboardMaterial::ParticleBillboardMaterial(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    defaultTexture(sAssetManager.GetTexture2D("pixel_diffuse")),
    diffuseTexture(nullptr),
    diffuseColor(1.f, 1.f, 1.f)
{
}

ParticleBillboardMaterial::~ParticleBillboardMaterial()
{
}

void ParticleBillboardMaterial::PreRender(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        glm::mat4 translationMatrix = renderer->GetTransform()->GetTranslationMatrix();
        glm::mat4 rotationMatrix = glm::inverse(glm::lookAt
        (
            glm::vec3(0.f),
            -camera->GetTransform()->GetDirectionBackward(),
            glm::vec3(0.f, 1.f, 0.f)
        ));

        shaderProgram->SetUniformMatrix4fv(0 /* Translation Matrix */, translationMatrix);
        shaderProgram->SetUniformMatrix4fv(1 /* Rotation Matrix */, rotationMatrix);
        shaderProgram->SetUniformMatrix4fv(2 /* View Projection Matrix */, camera->GetViewProjectionMatrix());
        shaderProgram->SetUniform1i(3 /* Diffuse Texture */, 0 /* Unit */);
        shaderProgram->SetUniform3fv(4 /* Diffuse Color */, diffuseColor);

        if (diffuseTexture)
        {
            diffuseTexture->ActivateAndBind(0 /* Unit */);
        }
        else if (defaultTexture)
        {
            defaultTexture->ActivateAndBind(0 /* Unit */);
        }
    }
}
