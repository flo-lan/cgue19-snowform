#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

struct Particle
{
	vec4 Position; // w = Size
	vec4 Velocity; // w = RemainingLifetime
	vec4 Random01;
};

layout(std430, binding = 0) buffer ParticleBuffer
{
	Particle particles[];
};

layout(location =  0) uniform float deltaTime;
layout(location =  1) uniform int maxParticles;
layout(location =  2) uniform vec3 minRelativeEmitPosition;
layout(location =  3) uniform vec3 maxRelativeEmitPosition;
layout(location =  4) uniform vec3 minParticleVelocity;
layout(location =  5) uniform vec3 maxParticleVelocity;
layout(location =  6) uniform float minParticleSize;
layout(location =  7) uniform float maxParticleSize;
layout(location =  8) uniform float minParticleLifetime;
layout(location =  9) uniform float maxParticleLifetime;
layout(location = 10) uniform vec2 randomSeed01;

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

//float random(vec2 co)
//{
//	return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
//}

vec2 random(vec2 co)
{
	return vec2
	(
		fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453),
		fract(sin(dot(((co + randomSeed01) / 2.0).xy, vec2(12.9898,78.233))) * 43758.5453)
	);
}

void main()
{
	uint gid = gl_GlobalInvocationID.x;

	if (gid >= maxParticles)
	{
		return;
	}

	Particle p = particles[gid];

	p.Velocity.w -= deltaTime;

	if (p.Velocity.w <= 0.0)
	{
		vec2 rnd1 = p.Random01.xy; p.Random01.xy = random(p.Random01.xy);
		vec2 rnd2 = p.Random01.xy; p.Random01.xy = random(p.Random01.xy);
		vec2 rnd3 = p.Random01.xy; p.Random01.xy = random(p.Random01.xy);
		vec2 rnd4 = p.Random01.xy; p.Random01.xy = random(p.Random01.xy);

		p.Position.xyz = (maxRelativeEmitPosition - minRelativeEmitPosition) * vec3(rnd1.x, rnd1.y, rnd2.x) + minRelativeEmitPosition;
		p.Position.w   = (maxParticleSize         - minParticleSize)         * rnd2.y                       + minParticleSize;
		p.Velocity.xyz = (maxParticleVelocity     - minParticleVelocity)     * vec3(rnd3.x, rnd3.y, rnd4.x) + minParticleVelocity;
		p.Velocity.w   = (maxParticleLifetime     - minParticleLifetime)     * rnd4.y                       + minParticleLifetime;
	}
	else
	{
		p.Position.xyz += p.Velocity.xyz * deltaTime;
	}

	particles[gid] = p;
}
