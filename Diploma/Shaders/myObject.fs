#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D diffuse;
uniform sampler2D specular;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight;

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// uniform SpotLight spotLight;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);
vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);
vec3 calcSpotLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 diffMap = vec3(texture(diffuse0, TexCoord));
    vec3 specMap = vec3(texture(specular0, TexCoord));

    // result
    FragColor = vec4(calcSpotLight(norm, viewDir, diffMap, specMap), 1.0);
}

vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
    // ambient
    vec3 ambient = dirLight.ambient * diffMap;

    // diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * (diff * diffMap);

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
    vec3 specular = dirLight.specular * (spec * specMap);

    return vec3(ambient + diffuse + spec);
}

vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
    // ambient
    vec3 ambient = pointLight.ambient * diffMap;

    //diffuse
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * (diff * diffMap);

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
    vec3 specular = pointLight.specular * (spec * specMap);

    // result
    return vec3(ambient + diffuse + specular);
}

vec3 calcSpotLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
    vec3 lightDir = normalize(spotLight.position - FragPos);
    float theta = dot(lightDir, normalize(-spotLight.direction));

    // ambient
    vec3 ambient = spotLight.ambient * diffMap;

    if (theta > spotLight.outerCutOff)
    {
        // if in cutoff, render light

        // diffuse
        float diff = max (dot(norm, lightDir), 0.0);
        vec3 diffuse = spotLight.diffuse * (doff * diffMap);

        // specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
        vec3 specular = spotLight.specular * (spec * specMap);

        float intensity = (theta - spotLight.outerCutOff) / (spotLight.cutOff - spotLight.outerCutOff);
        intensity = clamp(intensity, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;

        return vec3(ambient + diffuse + specular);
    }
    else {
        // render just ambient
        return ambient;
    }
}