// Fragment Shader
#version 430

layout(location = 0) uniform vec3 color;

in vec3 fragColor;

out vec4 colorOut;

void main()
{
    colorOut = vec4(color * fragColor, 1);
}
