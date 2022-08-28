#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

struct Material{
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
};

struct Light {
vec3 position;  
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;  



void main()
{
    // ambient:
    vec3 ambient = material.ambient * light.ambient;
    // diffuse:
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = material.diffuse * diff * light.diffuse;
    // specular:
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
    // resulting fragColor:
    vec3 result = (ambient+diffuse+specular);
    FragColor = vec4(result, 1.0);
}