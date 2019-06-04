// Vertex Shader
#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 color;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragUV;
out vec3 fragColor;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 viewProjection;

void main()
{
    fragPosition = vec3(model * vec4(position, 1.0));
    fragNormal = mat3(transpose(inverse(model))) * normal;
    fragNormal = normalize(fragNormal);
    fragUV = uv;
	fragColor = color;

    gl_Position = viewProjection * vec4(fragPosition, 1);
}
