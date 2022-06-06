#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normalPosition;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * normalPosition;
    // Normal = normalPosition;
    FragPos = vec3(vec4(vertexPosition, 1.0));

    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);    
}