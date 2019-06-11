#include "ShadowMapMaterial.h"
#include "ShaderProgram.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

ShadowMapMaterial::ShadowMapMaterial(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    shadowMapProjection(1.f)
{
}

ShadowMapMaterial::~ShadowMapMaterial()
{
}

void ShadowMapMaterial::PreRender(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(1 /* Model Matrix */, renderer->GetTransform()->GetModelMatrix());
        shaderProgram->SetUniformMatrix4fv(2 /* Shadow Map Projection Matrix */, shadowMapProjection);
    }
}
