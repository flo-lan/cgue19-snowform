// Vertex Shader
#version 430

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;

out vec2 fragUV;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    fragUV = uv;

    gl_Position = projection * model * vec4(position.xy, 0.0, 1.0);
}
