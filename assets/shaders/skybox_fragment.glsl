// Fragment Shader
#version 430

in vec3 fragTexCoord0;

out vec4 colorOut;

layout(location = 3) uniform samplerCube cubemapTexture;

void main()
{
	colorOut = texture(cubemapTexture, fragTexCoord0);
}
