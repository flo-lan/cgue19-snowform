#pragma once

#include "Material.h"
#include "LightConstants.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class Texture2D;

class StandardMaterial : public Material
{
public:
    StandardMaterial(std::string const& name, ShaderProgram* shaderProgram);
    virtual ~StandardMaterial();

    virtual void PreRender(CameraComponent* camera, MeshRendererComponent* renderer);

    void SetDiffuseColor(glm::vec3 value) { diffuseColor = value; }
    void SetSpecularColor(glm::vec3 value) { specularColor = value; }

    void SetDiffuseTexture(Texture2D* value) { diffuseTexture = value; }
    void SetSpecularTexture(Texture2D* value) { specularTexture = value; }

    void SetAmbientReflectionConstant(float value) { ambientReflectionConstant = value; }
    void SetDiffuseReflectionConstant(float value) { diffuseReflectionConstant = value; }
    void SetSpecularReflectionConstant(float value) { specularReflectionConstant = value; }

    void SetShininess(float value) { shininess = value; }

private:
    GLuint locViewPosition;
    GLuint locMaterialDiffuse;
    GLuint locMaterialSpecular;
    GLuint locMaterialDiffuseTexture;
    GLuint locMaterialSpecularTexture;
    GLuint locMaterialAmbientReflectionConstant;
    GLuint locMaterialDiffuseReflectionConstant;
    GLuint locMaterialSpecularReflectionConstant;
    GLuint locMaterialShininess;
    GLuint locAmbientLightAmbient;
    GLuint locDirectionalLightDirection[MAX_DIRECTIONAL_LIGHT_COUNT];
    GLuint locDirectionalLightDiffuse[MAX_DIRECTIONAL_LIGHT_COUNT];
    GLuint locDirectionalLightSpecular[MAX_DIRECTIONAL_LIGHT_COUNT];
    GLuint locDirectionalLightShadowEnabled[MAX_DIRECTIONAL_LIGHT_COUNT];
    GLuint locDirectionalLightShadowMaps[MAX_DIRECTIONAL_LIGHT_COUNT * NUM_DIRECTIONAL_SHADOW_CASCADES];
    GLuint locDirectionalLightShadowMapProjections[MAX_DIRECTIONAL_LIGHT_COUNT * NUM_DIRECTIONAL_SHADOW_CASCADES];
    GLuint locDirectionalLightShadowMapBounds[MAX_DIRECTIONAL_LIGHT_COUNT * NUM_DIRECTIONAL_SHADOW_CASCADES];
    GLuint locPointLightPosition[MAX_POINT_LIGHT_COUNT];
    GLuint locPointLightConstant[MAX_POINT_LIGHT_COUNT];
    GLuint locPointLightLinear[MAX_POINT_LIGHT_COUNT];
    GLuint locPointLightQuadratic[MAX_POINT_LIGHT_COUNT];
    GLuint locPointLightDiffuse[MAX_POINT_LIGHT_COUNT];
    GLuint locPointLightSpecular[MAX_POINT_LIGHT_COUNT];
    GLuint locSpotLightPosition[MAX_SPOT_LIGHT_COUNT];
    GLuint locSpotLightDirection[MAX_SPOT_LIGHT_COUNT];
    GLuint locSpotLightInnerCutOff[MAX_SPOT_LIGHT_COUNT];
    GLuint locSpotLightOuterCutOff[MAX_SPOT_LIGHT_COUNT];
    GLuint locSpotLightConstant[MAX_SPOT_LIGHT_COUNT];
    GLuint locSpotLightLinear[MAX_SPOT_LIGHT_COUNT];
    GLuint locSpotLightQuadratic[MAX_SPOT_LIGHT_COUNT];
    GLuint locSpotLightDiffuse[MAX_SPOT_LIGHT_COUNT];
    GLuint locSpotLightSpecular[MAX_SPOT_LIGHT_COUNT];

    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    Texture2D* defaultTexture;
    Texture2D* diffuseTexture;
    Texture2D* specularTexture;

    float ambientReflectionConstant;
    float diffuseReflectionConstant;
    float specularReflectionConstant;

    float shininess;
};
