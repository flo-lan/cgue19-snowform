#version 430

in vec2 fragUV;

out vec4 colorOut;

layout(location = 3) uniform sampler2D diffuseTexture;
layout(location = 4) uniform vec3 diffuseColor;

void main()
{
	colorOut = texture(diffuseTexture, fragUV) * vec4(diffuseColor, 1.0);
}
