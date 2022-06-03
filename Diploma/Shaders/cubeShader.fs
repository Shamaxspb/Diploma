#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor; 
    // FragColor = vec4(0.79f, 0.42f, 0.8f, 1.0f);
    // FragColor = vec4(lightColor, 1.0f);
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0f);
}