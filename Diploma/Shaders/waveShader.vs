#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
// add layout for vertexNormal
// add out vec3 Normal
// add out vec3 FragPosition

// out vec4 color; // QUESTIONABLE

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    // add FragPosition = vec3(model * vec3(vertexPosition, 1.0));
    // color = vec4(vertexColor, 1.0); // actual

    // light testing
    // color = vec4(0.39, 0.98, 1.0, 1.0); // QUESTIONABLE
}