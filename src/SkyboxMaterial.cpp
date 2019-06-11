#include "SkyboxMaterial.h"
#include "ShaderProgram.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Cubemap.h"
#include <glm/gtc/matrix_transform.hpp>

SkyboxMaterial::SkyboxMaterial(std::string const& name, ShaderProgram* shaderProgram) :
    Material(name, shaderProgram),
    cubemap(nullptr),
    prevCullFaceMode(0),
    prevDepthFuncMode(0)
{
}

SkyboxMaterial::~SkyboxMaterial()
{
}

void SkyboxMaterial::PreRender(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        glm::mat4 worldMatrix = glm::translate(glm::mat4(1.f), camera->GetPosition()) * glm::scale(glm::mat4(1.f), renderer->GetTransform()->GetScale());
        shaderProgram->SetUniformMatrix4fv(1 /* View Projection Matrix */, camera->GetViewProjectionMatrix());
        shaderProgram->SetUniformMatrix4fv(2 /* World Projection Matrix */, worldMatrix);
        shaderProgram->SetUniform1i(3 /* Cubemap */, 0);

        if (cubemap)
        {
            cubemap->ActivateAndBind(0);
        }
    }

    glGetIntegerv(GL_CULL_FACE_MODE, &prevCullFaceMode);
    glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
}

void SkyboxMaterial::PostRender(CameraComponent* camera, MeshRendererComponent* renderer)
{
    glCullFace(prevCullFaceMode);
    glDepthFunc(prevDepthFuncMode);
}
