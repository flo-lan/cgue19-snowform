// Vertex Shader
#version 430

layout(location = 0) in vec3 position;

out vec3 fragTexCoord0;

layout(location = 1) uniform mat4 viewProjection;
layout(location = 2) uniform mat4 worldProjection;

void main()
{
	vec4 worldViewProjectionPosition = viewProjection * worldProjection * vec4(position, 1);
    gl_Position = worldViewProjectionPosition.xyww;
	fragTexCoord0 = position;
}
