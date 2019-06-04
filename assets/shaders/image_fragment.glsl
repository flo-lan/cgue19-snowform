// Fragment Shader
#version 430

layout(location = 2) uniform sampler2D imageTexture;
layout(location = 3) uniform vec4 imageColor;

in vec2 fragUV;

out vec4 colorOut;

void main()
{
	vec4 imageTextureColor = texture(imageTexture, fragUV);
    colorOut = imageTextureColor * imageColor;
}
