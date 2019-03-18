// Vertex Shader
#version 430

layout(location = 0) in vec3 position;
layout(location = 3) in vec3 color;

out vec3 fragColor;

layout(location = 1) uniform mat4 model;
layout(location = 2) uniform mat4 viewProjection;

void main()
{
	fragColor = color;

    gl_Position = viewProjection * model * vec4(position, 1);
}
