#include "SimpleMaterial.h"
#include "ShaderProgram.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

SimpleMaterial::SimpleMaterial(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    color(1.f, 1.f, 1.f)
{
}

SimpleMaterial::~SimpleMaterial()
{
}

void SimpleMaterial::SetUniforms(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniform3fv(0 /* Color */, color);
        shaderProgram->SetUniformMatrix4fv(1 /* Model Matrix */, renderer->GetTransform()->GetModelMatrix());
        shaderProgram->SetUniformMatrix4fv(2 /* View Projection Matrix */, camera->GetViewProjectionMatrix());
    }
}
