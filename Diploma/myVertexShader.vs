#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;

out vec4 color;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // gl_Position = transform * vec4(vertexPosition, 1.0f);
    // gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    gl_Position = transform * (projection * view * model * vec4(vertexPosition, 1.0));

    // color = vec4(0.2, 0.2, 0.2, 1.0);
    // color = vec4(0.0, 0.44, 0.45, 1.0);
    // test color
    color = vec4(0.16, 0.16, 0.16, 1.0);
    
}