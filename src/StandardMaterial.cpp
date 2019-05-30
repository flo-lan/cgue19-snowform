#include "StandardMaterial.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "AssetManager.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "RenderSettings.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "Settings.h"

StandardMaterial::StandardMaterial(std::string const& _name, ShaderProgram* _shaderProgram) :
    Material(_name, _shaderProgram),
    diffuseColor(1.f, 1.f, 1.f),
    specularColor(1.f, 1.f, 1.f),
    defaultTexture(sAssetManager.GetTexture2D("pixel_diffuse")),
    diffuseTexture(nullptr),
    specularTexture(nullptr),
    ambientReflectionConstant(1.f),
    diffuseReflectionConstant(1.f),
    specularReflectionConstant(1.f),
    shininess(32.f)
{
}

StandardMaterial::~StandardMaterial()
{
}

void StandardMaterial::SetUniforms(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(shaderProgram->GetUniformLocation("model"), renderer->GetTransform()->GetModelMatrix());
        shaderProgram->SetUniformMatrix4fv(shaderProgram->GetUniformLocation("viewProjection"), camera->GetViewProjectionMatrix());
        shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation("viewPosition"), camera->GetPosition());

        shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation("material.diffuse"), diffuseColor);
        shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation("material.specular"), specularColor);

        int textureUnit = 0;

        if (defaultTexture)
        {
            defaultTexture->ActivateAndBind(textureUnit);

            ++textureUnit;
        }

        if (diffuseTexture)
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("diffuseTexture"), textureUnit);

            diffuseTexture->ActivateAndBind(textureUnit);

            ++textureUnit;
        }
        else
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("diffuseTexture"), 0 /* Unit */);
        }

        if (specularTexture)
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("specularTexture"), textureUnit);

            specularTexture->ActivateAndBind(textureUnit);

            ++textureUnit;
        }
        else
        {
            shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation("specularTexture"), 0 /* Unit */);
        }

        shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation("material.ambientReflectionConstant"), ambientReflectionConstant);
        shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation("material.diffuseReflectionConstant"), diffuseReflectionConstant);
        shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation("material.specularReflectionConstant"), specularReflectionConstant);

        shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation("material.shininess"), shininess);

        shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation("ambientLight.ambient"), RenderSettings::AmbientColor * RenderSettings::AmbientIntensity * sSettings.getBrightness());

        uint32_t i = 0;
        for (std::vector<DirectionalLightComponent*>::const_iterator itr = renderer->GetDirectionalLights().begin(); itr != renderer->GetDirectionalLights().end(); itr++, i++)
        {
            DirectionalLightComponent* light = *itr;

            if (light->IsShadowEnabled())
            {
                shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].shadowEnabled").c_str()), 1);

                for (int cascadeIndex = 0; cascadeIndex < NUM_DIRECTIONAL_SHADOW_CASCADES; cascadeIndex++)
                {
                    shaderProgram->SetUniformMatrix4fv(shaderProgram->GetUniformLocation(std::string("directionalShadowMapProjections[" + std::to_string(i * NUM_DIRECTIONAL_SHADOW_CASCADES + cascadeIndex) + "]").c_str()), light->GetShadowMapProjection(cascadeIndex));
                    shaderProgram->SetUniform4fv(shaderProgram->GetUniformLocation(std::string("directionalShadowMapCascadeBounds[" + std::to_string(i * NUM_DIRECTIONAL_SHADOW_CASCADES + cascadeIndex) + "]").c_str()), light->GetShadowMapCascadeBounds(cascadeIndex));
                    shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation(std::string("directionalShadowMaps[" + std::to_string(i * NUM_DIRECTIONAL_SHADOW_CASCADES + cascadeIndex) + "]").c_str()), textureUnit);

                    light->ActivateAndBindShadowMap(textureUnit, cascadeIndex);

                    ++textureUnit;
                }
            }
            else
            {
                shaderProgram->SetUniform1i(shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].shadowEnabled").c_str()), 0);
            }

            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].direction").c_str()), light->GetDirection());

            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].diffuse").c_str()),  light->GetColor() * light->GetIntensity());
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].specular").c_str()), light->GetColor() * light->GetIntensity());
        }

        for (; i < MAX_DIRECTIONAL_LIGHT_COUNT; i++)
        {
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].diffuse").c_str()),  glm::vec3(0.f));
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].specular").c_str()), glm::vec3(0.f));
        }

        i = 0;
        for (std::vector<PointLightComponent*>::const_iterator itr = renderer->GetPointLights().begin(); itr != renderer->GetPointLights().end(); itr++, i++)
        {
            PointLightComponent* light = *itr;

            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].position").c_str()), light->GetPosition());

            shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].constant").c_str()), light->GetConstant());
            shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].linear").c_str()), light->GetLinear());
            shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].quadratic").c_str()), light->GetQuadratic());

            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].diffuse").c_str()), light->GetColor() * light->GetIntensity());
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].specular").c_str()), light->GetColor() * light->GetIntensity());
        }

        for (; i < MAX_POINT_LIGHT_COUNT; i++)
        {
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].diffuse").c_str()), glm::vec3(0.f));
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].specular").c_str()), glm::vec3(0.f));
        }

        i = 0;
        for (std::vector<SpotLightComponent*>::const_iterator itr = renderer->GetSpotLights().begin(); itr != renderer->GetSpotLights().end(); itr++, i++)
        {
            SpotLightComponent* light = *itr;

            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].position").c_str()), light->GetPosition());
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].direction").c_str()), light->GetDirection());

            shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].innerCutOff").c_str()), light->GetInnerCutOff());
            shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].outerCutOff").c_str()), light->GetOuterCutOff());

            shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].constant").c_str()), light->GetConstant());
            shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].linear").c_str()), light->GetLinear());
            shaderProgram->SetUniform1fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].quadratic").c_str()), light->GetQuadratic());

            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].diffuse").c_str()), light->GetColor() * light->GetIntensity());
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].specular").c_str()), light->GetColor() * light->GetIntensity());
        }

        for (; i < MAX_SPOT_LIGHT_COUNT; i++)
        {
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].diffuse").c_str()), glm::vec3(0.f));
            shaderProgram->SetUniform3fv(shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].specular").c_str()), glm::vec3(0.f));
        }
    }
}
