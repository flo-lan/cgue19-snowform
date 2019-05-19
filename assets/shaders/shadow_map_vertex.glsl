// Vertex Shader
#version 430

layout(location = 0) in vec3 position;

layout(location = 1) uniform mat4 model;
layout(location = 2) uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1);
}
