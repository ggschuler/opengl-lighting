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

struct Light {
vec3 position;
vec3 direction;  
vec3 ambient;
vec3 diffuse;
vec3 specular;

float constant;
float linear;
float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;  



void main()
{
    // ambient:
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    // diffuse:
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;
    // specular:
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;
    // attenuation math:
    float distance = length(light.position - FragPos);
    float attenuation = 1 / (light.constant + (light.linear*distance) + (light.quadratic*pow(distance,2)));
    // multiply with attenuation:
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    // resulting fragColor:
    vec3 result = (ambient+diffuse+specular);
    FragColor = vec4(result, 1.0);
}