// Fragment Shader
#version 430

uniform sampler2D cutOffTexture;
uniform sampler2D imageTexture;
uniform vec4 imageColor;
uniform float cutOff;

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
