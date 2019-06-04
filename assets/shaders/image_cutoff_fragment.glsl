// Fragment Shader
#version 430

layout(location = 2) uniform sampler2D cutOffTexture;
layout(location = 3) uniform sampler2D imageTexture;
layout(location = 4) uniform vec4 imageColor;
layout(location = 5) uniform float cutOff;

in vec2 fragUV;

out vec4 colorOut;

void main()
{
	if (cutOff <= texture(cutOffTexture, fragUV).a)
	{
		colorOut = texture(imageTexture, fragUV) * imageColor;
	}
	else
	{
		colorOut = vec4(0.0, 0.0, 0.0, 0.0);
	}
}
