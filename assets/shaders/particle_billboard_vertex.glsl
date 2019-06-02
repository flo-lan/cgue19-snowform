#version 430

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;
layout(location = 4) in vec4 relativeParticlePosition; // w = Size

out vec2 fragUV;

layout(location = 0) uniform mat4 translationMatrix;
layout(location = 1) uniform mat4 rotationMatrix;
layout(location = 2) uniform mat4 viewProjection;

void main(void)
{
	fragUV = uv;

	mat4 relativeParticleTranslation = mat4
	(
		vec4(1.0, 0.0, 0.0, 0.0),
		vec4(0.0, 1.0, 0.0, 0.0),
		vec4(0.0, 0.0, 1.0, 0.0),
		vec4(relativeParticlePosition.xyz, 1.0)
	);

	mat4 particleScaleMatrix = mat4
	(
		vec4(relativeParticlePosition.w, 0.0, 0.0, 0.0),
		vec4(0.0, relativeParticlePosition.w, 0.0, 0.0),
		vec4(0.0, 0.0, relativeParticlePosition.w, 0.0),
		vec4(0.0, 0.0, 0.0,                        1.0)
	);

	gl_Position = viewProjection * relativeParticleTranslation * translationMatrix * particleScaleMatrix * rotationMatrix * vec4(position, 1.0);
}
