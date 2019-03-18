// Fragment Shader
#version 430

struct Material
{
    vec3 diffuse;
    vec3 specular;

    sampler2D diffuseTexture;
    sampler2D specularTexture;

    float ambientReflectionConstant;
    float diffuseReflectionConstant;
    float specularReflectionConstant;

    float shininess;
};

struct AmbientLight
{
    vec3 ambient;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float innerCutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 diffuse;
    vec3 specular;
};

#define MAX_DIRECTIONAL_LIGHT_COUNT 4
#define MAX_POINT_LIGHT_COUNT 4
#define MAX_SPOT_LIGHT_COUNT 2

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragUV;
in vec3 fragColor;

out vec4 colorOut;

uniform vec3 viewPosition;
uniform Material material;
uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_COUNT];
uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform SpotLight spotLights[MAX_SPOT_LIGHT_COUNT];

// Credits: https://learnopengl.com/Lighting/Multiple-lights

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec4 diffuseTextureColor, vec4 specularTextureColor)
{
    vec3 lightDirection = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);
    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    // Combine results
    vec3 diffuse  = light.diffuse  * material.diffuseReflectionConstant  * material.diffuse  * diff * diffuseTextureColor.rgb * fragColor;
    vec3 specular = light.specular * material.specularReflectionConstant * material.specular * spec * specularTextureColor.rgb;
    return (diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection, vec4 diffuseTextureColor, vec4 specularTextureColor)
{
    vec3 lightDirection = normalize(light.position - fragPosition);
    // Diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);
    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / max(light.constant + light.linear * distance + light.quadratic * (distance * distance), 1.0);
    // Combine results
    vec3 diffuse  = attenuation * light.diffuse  * material.diffuseReflectionConstant  * material.diffuse  * diff * diffuseTextureColor.rgb * fragColor;
    vec3 specular = attenuation * light.specular * material.specularReflectionConstant * material.specular * spec * specularTextureColor.rgb;
    return (diffuse + specular);
}

// Credits: https://learnopengl.com/Lighting/Light-casters

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection, vec4 diffuseTextureColor, vec4 specularTextureColor)
{
    vec3 lightDirection = normalize(light.position - fragPosition);
    // Diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);
    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0 / max(light.constant + light.linear * distance + light.quadratic * (distance * distance), 1.0);
    // Spotlight
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec3 diffuse  = intensity * attenuation * light.diffuse  * material.diffuseReflectionConstant  * material.diffuse  * diff * diffuseTextureColor.rgb * fragColor;
    vec3 specular = intensity * attenuation * light.specular * material.specularReflectionConstant * material.specular * spec * specularTextureColor.rgb;
    return (diffuse + specular);
}

void main()
{
    vec4 diffuseTextureColor = texture(material.diffuseTexture, fragUV);
    vec4 specularTextureColor = texture(material.specularTexture, fragUV);
    vec3 result = ambientLight.ambient * material.ambientReflectionConstant * material.diffuse * diffuseTextureColor.rgb * fragColor;
    vec3 normal = normalize(fragNormal); // Important, because length of fragNormal can be != 1 after interpolation
    vec3 viewDirection = normalize(viewPosition - fragPosition);

    for (int i = 0; i < MAX_DIRECTIONAL_LIGHT_COUNT; i++)
    {
        result += CalculateDirectionalLight(directionalLights[i], normal, viewDirection, diffuseTextureColor, specularTextureColor);
    }

    for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++)
    {
        result += CalculatePointLight(pointLights[i], normal, viewDirection, diffuseTextureColor, specularTextureColor);
    }

    for (int i = 0; i < MAX_SPOT_LIGHT_COUNT; i++)
    {
        result += CalculateSpotLight(spotLights[i], normal, viewDirection, diffuseTextureColor, specularTextureColor);
    }

    colorOut = vec4(result, 1.0);
}
