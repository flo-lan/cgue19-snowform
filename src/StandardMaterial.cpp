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
    shaderProgram->Use();

    locViewPosition = shaderProgram->GetUniformLocation("viewPosition");
    locMaterialDiffuse = shaderProgram->GetUniformLocation("material.diffuse");
    locMaterialSpecular = shaderProgram->GetUniformLocation("material.specular");
    locMaterialDiffuseTexture = shaderProgram->GetUniformLocation("diffuseTexture");
    locMaterialSpecularTexture = shaderProgram->GetUniformLocation("specularTexture");
    locMaterialAmbientReflectionConstant = shaderProgram->GetUniformLocation("material.ambientReflectionConstant");
    locMaterialDiffuseReflectionConstant = shaderProgram->GetUniformLocation("material.diffuseReflectionConstant");
    locMaterialSpecularReflectionConstant = shaderProgram->GetUniformLocation("material.specularReflectionConstant");
    locMaterialShininess = shaderProgram->GetUniformLocation("material.shininess");
    locAmbientLightAmbient = shaderProgram->GetUniformLocation("ambientLight.ambient");

    for (int i = 0; i < MAX_DIRECTIONAL_LIGHT_COUNT; i++)
    {
        locDirectionalLightDirection[i] = shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].direction").c_str());
        locDirectionalLightDiffuse[i] = shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].diffuse").c_str());
        locDirectionalLightSpecular[i] = shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].specular").c_str());
        locDirectionalLightShadowEnabled[i] = shaderProgram->GetUniformLocation(std::string("directionalLights[" + std::to_string(i) + "].shadowEnabled").c_str());

        for (int j = 0, k = i * NUM_DIRECTIONAL_SHADOW_CASCADES; j < NUM_DIRECTIONAL_SHADOW_CASCADES; j++, k++)
        {
            locDirectionalLightShadowMaps[k] = shaderProgram->GetUniformLocation(std::string("directionalShadowMaps[" + std::to_string(k) + "]").c_str());
            locDirectionalLightShadowMapProjections[k] = shaderProgram->GetUniformLocation(std::string("directionalShadowMapProjections[" + std::to_string(k) + "]").c_str());
            locDirectionalLightShadowMapBounds[k] = shaderProgram->GetUniformLocation(std::string("directionalShadowMapBounds[" + std::to_string(k) + "]").c_str());
        }
    }

    for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++)
    {
        locPointLightPosition[i] = shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].position").c_str());
        locPointLightConstant[i] = shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].constant").c_str());
        locPointLightLinear[i] = shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].linear").c_str());
        locPointLightQuadratic[i] = shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].quadratic").c_str());
        locPointLightDiffuse[i] = shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].diffuse").c_str());
        locPointLightSpecular[i] = shaderProgram->GetUniformLocation(std::string("pointLights[" + std::to_string(i) + "].specular").c_str());
    }

    for (int i = 0; i < MAX_SPOT_LIGHT_COUNT; i++)
    {
        locSpotLightPosition[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].position").c_str());
        locSpotLightDirection[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].direction").c_str());
        locSpotLightInnerCutOff[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].innerCutOff").c_str());
        locSpotLightOuterCutOff[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].outerCutOff").c_str());
        locSpotLightConstant[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].constant").c_str());
        locSpotLightLinear[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].linear").c_str());
        locSpotLightQuadratic[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].quadratic").c_str());
        locSpotLightDiffuse[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].diffuse").c_str());
        locSpotLightSpecular[i] = shaderProgram->GetUniformLocation(std::string("spotLights[" + std::to_string(i) + "].specular").c_str());
    }
}

StandardMaterial::~StandardMaterial()
{
}

void StandardMaterial::PreRender(CameraComponent* camera, MeshRendererComponent* renderer)
{
    if (shaderProgram)
    {
        shaderProgram->SetUniformMatrix4fv(0, renderer->GetTransform()->GetModelMatrix());
        shaderProgram->SetUniformMatrix4fv(1, camera->GetViewProjectionMatrix());
        shaderProgram->SetUniform3fv(locViewPosition, camera->GetPosition());

        shaderProgram->SetUniform3fv(locMaterialDiffuse, diffuseColor);
        shaderProgram->SetUniform3fv(locMaterialSpecular, specularColor);

        int textureUnit = 0;

        if (defaultTexture)
        {
            defaultTexture->ActivateAndBind(textureUnit);

            ++textureUnit;
        }

        if (diffuseTexture)
        {
            shaderProgram->SetUniform1i(locMaterialDiffuseTexture, textureUnit);

            diffuseTexture->ActivateAndBind(textureUnit);

            ++textureUnit;
        }
        else
        {
            shaderProgram->SetUniform1i(locMaterialDiffuseTexture, 0 /* Unit */);
        }

        if (specularTexture)
        {
            shaderProgram->SetUniform1i(locMaterialSpecularTexture, textureUnit);

            specularTexture->ActivateAndBind(textureUnit);

            ++textureUnit;
        }
        else
        {
            shaderProgram->SetUniform1i(locMaterialSpecularTexture, 0 /* Unit */);
        }

        shaderProgram->SetUniform1fv(locMaterialAmbientReflectionConstant, ambientReflectionConstant);
        shaderProgram->SetUniform1fv(locMaterialDiffuseReflectionConstant, diffuseReflectionConstant);
        shaderProgram->SetUniform1fv(locMaterialSpecularReflectionConstant, specularReflectionConstant);

        shaderProgram->SetUniform1fv(locMaterialShininess, shininess);

        shaderProgram->SetUniform3fv(locAmbientLightAmbient, RenderSettings::AmbientColor * RenderSettings::AmbientIntensity * sSettings.getBrightness());

        int i = 0;
        for (std::vector<DirectionalLightComponent*>::const_iterator itr = renderer->GetDirectionalLights().begin(); itr != renderer->GetDirectionalLights().end(); itr++, i++)
        {
            DirectionalLightComponent* light = *itr;

            shaderProgram->SetUniform3fv(locDirectionalLightDirection[i], light->GetDirection());

            shaderProgram->SetUniform3fv(locDirectionalLightDiffuse[i],  light->GetColor() * light->GetIntensity());
            shaderProgram->SetUniform3fv(locDirectionalLightSpecular[i], light->GetColor() * light->GetIntensity());


            if (light->IsShadowEnabled())
            {
                shaderProgram->SetUniform1i(locDirectionalLightShadowEnabled[i], 1);

                for (int j = 0, k = i * NUM_DIRECTIONAL_SHADOW_CASCADES; j < NUM_DIRECTIONAL_SHADOW_CASCADES; j++, k++)
                {
                    shaderProgram->SetUniform1i(locDirectionalLightShadowMaps[k], textureUnit);
                    shaderProgram->SetUniformMatrix4fv(locDirectionalLightShadowMapProjections[k], light->GetShadowMapProjection(j));
                    shaderProgram->SetUniform4fv(locDirectionalLightShadowMapBounds[k], light->GetShadowMapBounds(j));

                    light->ActivateAndBindShadowMap(textureUnit, j);

                    ++textureUnit;
                }
            }
            else
            {
                shaderProgram->SetUniform1i(locDirectionalLightShadowEnabled[i], 0);
            }
        }

        for (; i < MAX_DIRECTIONAL_LIGHT_COUNT; i++)
        {
            shaderProgram->SetUniform3fv(locDirectionalLightDiffuse[i],  glm::vec3(0.f));
            shaderProgram->SetUniform3fv(locDirectionalLightSpecular[i], glm::vec3(0.f));
        }

        i = 0;
        for (std::vector<PointLightComponent*>::const_iterator itr = renderer->GetPointLights().begin(); itr != renderer->GetPointLights().end(); itr++, i++)
        {
            PointLightComponent* light = *itr;

            shaderProgram->SetUniform3fv(locPointLightPosition[i], light->GetPosition());

            shaderProgram->SetUniform1fv(locPointLightConstant[i], light->GetConstant());
            shaderProgram->SetUniform1fv(locPointLightLinear[i], light->GetLinear());
            shaderProgram->SetUniform1fv(locPointLightQuadratic[i], light->GetQuadratic());

            shaderProgram->SetUniform3fv(locPointLightDiffuse[i], light->GetColor() * light->GetIntensity());
            shaderProgram->SetUniform3fv(locPointLightSpecular[i], light->GetColor() * light->GetIntensity());
        }

        for (; i < MAX_POINT_LIGHT_COUNT; i++)
        {
            shaderProgram->SetUniform3fv(locPointLightDiffuse[i], glm::vec3(0.f));
            shaderProgram->SetUniform3fv(locPointLightSpecular[i], glm::vec3(0.f));
        }

        i = 0;
        for (std::vector<SpotLightComponent*>::const_iterator itr = renderer->GetSpotLights().begin(); itr != renderer->GetSpotLights().end(); itr++, i++)
        {
            SpotLightComponent* light = *itr;

            shaderProgram->SetUniform3fv(locSpotLightPosition[i], light->GetPosition());
            shaderProgram->SetUniform3fv(locSpotLightDirection[i], light->GetDirection());

            shaderProgram->SetUniform1fv(locSpotLightInnerCutOff[i], light->GetInnerCutOff());
            shaderProgram->SetUniform1fv(locSpotLightOuterCutOff[i], light->GetOuterCutOff());

            shaderProgram->SetUniform1fv(locSpotLightConstant[i], light->GetConstant());
            shaderProgram->SetUniform1fv(locSpotLightLinear[i], light->GetLinear());
            shaderProgram->SetUniform1fv(locSpotLightQuadratic[i], light->GetQuadratic());

            shaderProgram->SetUniform3fv(locSpotLightDiffuse[i], light->GetColor() * light->GetIntensity());
            shaderProgram->SetUniform3fv(locSpotLightSpecular[i], light->GetColor() * light->GetIntensity());
        }

        for (; i < MAX_SPOT_LIGHT_COUNT; i++)
        {
            shaderProgram->SetUniform3fv(locSpotLightDiffuse[i], glm::vec3(0.f));
            shaderProgram->SetUniform3fv(locSpotLightSpecular[i], glm::vec3(0.f));
        }
    }
}
