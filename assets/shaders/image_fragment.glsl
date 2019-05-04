// Fragment Shader
#version 430

uniform sampler2D imageTexture;
uniform vec4 imageColor;

in vec2 fragUV;

out vec4 colorOut;

void main()
{
	vec4 imageTextureColor = texture(imageTexture, fragUV);
    colorOut = imageTextureColor * imageColor;
}
