#include "ShadowMapMaterial.h"
#include "ShaderProgram.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

ShadowMapMaterial::ShadowMapMaterial(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    lightSpaceMatrix(1.f)
{
}

ShadowMapMaterial::~ShadowMapMaterial()
{
}

void ShadowMapMaterial::SetUniforms(MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(1 /* Model Matrix */, renderer->GetTransform() ? renderer->GetTransform()->GetModelMatrix() : glm::mat4(1.f));
        shaderProgram->SetUniformMatrix4fv(2 /* Light Space Matrix */, lightSpaceMatrix);
    }
}
