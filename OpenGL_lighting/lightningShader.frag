#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    // diffuse lightning calculations:
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos-FragPos);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff*lightColor;
    // ambient lightning calculations:  
    float ambientStrenght = 0.3;
    vec3 ambient = ambientStrenght * lightColor;
    vec3 result = (ambient+diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}