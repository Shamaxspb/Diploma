#version 330 core

// in vec4 color;
// add in vec3 Normal
// add in vec3 FragPosition

out vec4 FragColor;

// vec3 objectColor;
uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;
// uniform viewPos;


void main()
{
    // FragColor = color;

    // light testing
    float ambientStrength = 0.25;
    // vec3 norm = normalize(Normal);
    // float specularStrength = 0.5;
    // vec3 viewDir = normalize(viewPos)
    // vec3 reflectDir = reflect(-lightDirection, norm)
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient * objectColor;
    // FragColor = vec4(lightColor * objectColor, 1.0);
    FragColor = vec4(result, 1.0);

    
    // vec3 lightDirection = normalize(lightPosition - FragPosition);
}