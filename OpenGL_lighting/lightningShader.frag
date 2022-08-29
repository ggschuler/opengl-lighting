#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

struct Material{
sampler2D diffuse;
sampler2D specular;
float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float incutoff;
    float outcutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4  

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight sLight;

vec3 dirLightContribution(DirLight dLight, vec3 normal, vec3 viewDir);
vec3 pointLightContribution(PointLight pLight, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 spotLightContribution(SpotLight sLight, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
    vec3 normal  = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = dirLightContribution(dLight, normal, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += pointLightContribution(pointLights[i], normal, viewDir, FragPos);
    }
    result += spotLightContribution(sLight, normal, viewDir, FragPos);
    FragColor = vec4(result, 1.0f);
}

vec3 dirLightContribution(DirLight dLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir   = normalize(-dLight.direction);
    float diff      = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 ambient    = dLight.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse    = dLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular   = dLight.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient+diffuse+specular);
}

vec3 pointLightContribution(PointLight pLight, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir     = normalize(-pLight.position - fragPos);
    float diff        = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir   = reflect(-lightDir, normal);
    float spec        = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    float distance    = length(pLight.position - fragPos);
    float attenuation = 1 / (pLight.constant + (pLight.linear*distance) + (pLight.quadratic*pow(distance, 2)));
    vec3 ambient      = pLight.ambient  * vec3(texture(material.diffuse, TexCoords))         * attenuation;
    vec3 diffuse      = pLight.diffuse  * diff * vec3(texture(material.diffuse, TexCoords))  * attenuation;
    vec3 specular     = pLight.specular * spec * vec3(texture(material.specular, TexCoords)) * attenuation;
    return (ambient+diffuse+specular);
}

vec3 spotLightContribution(SpotLight sLight, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir     = normalize(sLight.position - fragPos);
    float diff        = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir   = reflect(-lightDir, normal);
    float spec        = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    float distance = length(sLight.position - fragPos);
    float attenuation = 1.0 / (sLight.constant + sLight.linear * distance + sLight.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-sLight.direction)); 
    float epsilon = sLight.incutoff - sLight.outcutoff;
    float intensity = clamp((theta - sLight.outcutoff) / epsilon, 0.0, 1.0);
    vec3 ambient      = sLight.ambient  * vec3(texture(material.diffuse, TexCoords))         * intensity * attenuation;;
    vec3 diffuse      = sLight.diffuse  * diff * vec3(texture(material.diffuse, TexCoords))  * intensity * attenuation;;
    vec3 specular     = sLight.specular * spec * vec3(texture(material.specular, TexCoords)) * intensity * attenuation;;
    return (ambient+diffuse+specular);
}
    
    