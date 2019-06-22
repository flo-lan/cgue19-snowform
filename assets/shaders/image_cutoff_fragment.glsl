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
	float alpha = texture(cutOffTexture, fragUV).a;

	if (cutOff <= alpha)
	{
		colorOut = texture(imageTexture, fragUV) * imageColor;
	}
	else if (cutOff <= alpha + 0.05)
	{
		alpha = (alpha + 0.05 - cutOff) / 0.05;
		colorOut = texture(imageTexture, fragUV) * imageColor * vec4(1.0, 1.0, 1.0, alpha);
	}
	else
	{
		colorOut = vec4(0.0, 0.0, 0.0, 0.0);
	}
}
