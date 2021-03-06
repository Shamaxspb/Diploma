#version 330 core
// struct Material
// {
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//     float shininess;
// }

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
// uniform Material materal;
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    // specular
    float specularStrength = 0.75;
    vec3 viewDir = normalize(abs(viewPos - FragPos));
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 