#include "SimpleMaterial.h"
#include "ShaderProgram.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

SimpleMaterial::SimpleMaterial(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    color(0.f, 0.f, 0.f)
{
}

SimpleMaterial::~SimpleMaterial()
{
}

void SimpleMaterial::SetUniforms(MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniform3fv(0 /* Color */, color);
        shaderProgram->SetUniformMatrix4fv(1 /* Model Matrix */, renderer->GetTransform() ? renderer->GetTransform()->GetModelMatrix() : glm::mat4(1.f));
        shaderProgram->SetUniformMatrix4fv(2 /* View Projection Matrix */, renderer->GetCamera() ? renderer->GetCamera()->GetViewProjectionMatrix() : glm::mat4(1.f));
    }
}
