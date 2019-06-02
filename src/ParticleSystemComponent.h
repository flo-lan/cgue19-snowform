#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

class Material;
class MeshRendererComponent;
class ShaderProgram;
class Mesh;

struct Particle
{
    glm::vec4 Position; // w = Size
    glm::vec4 Velocity; // w = RemainingLifetime
    glm::vec4 Random01;

    Particle() :
        Position(0.f),
        Velocity(0.f),
        Random01((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0.f, 0.f)
    {}
};

#define WORK_GROUP_SIZE 256
#define PARTICLE_BUFFER_SIZE 10000

class ParticleSystemComponent : public Component
{
public:
    ParticleSystemComponent(GameObject* owner);
    virtual ~ParticleSystemComponent();

    virtual void LateUpdate();
    virtual void OnDestroy();

    bool IsEnabled() const { return enabled; }
    void SetEnabled(bool enable) { this->enabled = enable; }

    void SetMinRelativeEmitPosition(glm::vec3 const& minRelativeEmitPosition) { this->minRelativeEmitPosition = minRelativeEmitPosition; }
    void SetMaxRelativeEmitPosition(glm::vec3 const& maxRelativeEmitPosition) { this->maxRelativeEmitPosition = maxRelativeEmitPosition; }
    void SetMinParticleVelocity(glm::vec3 const& minParticleVelocity) { this->minParticleVelocity = minParticleVelocity; }
    void SetMaxParticleVelocity(glm::vec3 const& maxParticleVelocity) { this->maxParticleVelocity = maxParticleVelocity; }
    void SetMinParticleSize(float minParticleSize) { this->minParticleSize = minParticleSize; }
    void SetMaxParticleSize(float maxParticleSize) { this->maxParticleSize = maxParticleSize; }
    void SetMinParticleLifetime(float minParticleLifetime) { this->minParticleLifetime = minParticleLifetime; }
    void SetMaxParticleLifetime(float maxParticleLifetime) { this->maxParticleLifetime = maxParticleLifetime; }
    void SetParticleCount(int particleCount);
    void SetParticleMaterial(Material* material);
    void SetParticleMesh(Mesh* mesh);

private:
    void CreateMeshRenderer();
    void DestroyMeshRenderer();
    void CreateVAOAndPBO();
    void DeleteVAOAndPBO();

    ShaderProgram* computeShaderProgram;
    MeshRendererComponent* meshRenderer;

    GLuint VAO;
    GLuint PBO;

    bool enabled;
    glm::vec3 minRelativeEmitPosition;
    glm::vec3 maxRelativeEmitPosition;
    glm::vec3 minParticleVelocity;
    glm::vec3 maxParticleVelocity;
    float minParticleSize;
    float maxParticleSize;
    float minParticleLifetime;
    float maxParticleLifetime;
    int particleCount;
    Particle particleBuffer[PARTICLE_BUFFER_SIZE];
};
