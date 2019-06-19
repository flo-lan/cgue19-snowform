#include "ParticleSystemComponent.h"
#include "MeshRendererComponent.h"
#include "ShaderProgram.h"
#include "AssetManager.h"
#include "Material.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Time.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* owner) :
    Component::Component(owner),
    computeShaderProgram(sAssetManager.GetShaderProgram("particle_system_compute_shader_program")),
    meshRenderer(nullptr),
    VAO(0),
    PBO(0),
    enabled(false),
    prewarmTime(0.f),
    minRelativeEmitPosition(0.f),
    maxRelativeEmitPosition(0.f),
    minParticleVelocity(-1.f),
    maxParticleVelocity(1.f),
    minParticleSize(1.f),
    maxParticleSize(1.f),
    minParticleLifetime(1.f),
    maxParticleLifetime(10.f),
    particleCount(0),
    particleBuffer{Particle()}
{
    fprintf(stdout, "Attached particle system component to game object '%s'!\n", GetOwner()->GetName().c_str());

    CreateMeshRenderer();
    CreateVAOAndPBO();
}

ParticleSystemComponent::~ParticleSystemComponent()
{
    fprintf(stdout, "Deleted particle system component from game object '%s'!\n", GetOwner()->GetName().c_str());

    DeleteVAOAndPBO();
}

void ParticleSystemComponent::OnStart()
{
    float remainingPrewarmTime = prewarmTime;

    for (; remainingPrewarmTime >= 1.f; remainingPrewarmTime -= 1.f)
    {
        Simulate(1.f);
    }

    if (remainingPrewarmTime > 0.f)
    {
        Simulate(remainingPrewarmTime);
    }
}

void ParticleSystemComponent::LateUpdate()
{
    Simulate(sTime.GetDeltaTime());
}

void ParticleSystemComponent::OnDestroy()
{
    DestroyMeshRenderer();
}

void ParticleSystemComponent::SetParticleCount(int particleCount)
{
    if (particleCount < 0)
    {
        particleCount = 0;
    }

    if (particleCount > PARTICLE_BUFFER_SIZE)
    {
        particleCount = PARTICLE_BUFFER_SIZE;
    }

    this->particleCount = particleCount;

    if (meshRenderer)
    {
        meshRenderer->SetEnabled(particleCount > 0);
        meshRenderer->SetInstanceCount(particleCount);
    }
}

void ParticleSystemComponent::SetParticleMaterial(Material* material)
{
    if (meshRenderer)
    {
        meshRenderer->SetMaterial(material);
    }
}

void ParticleSystemComponent::SetParticleMesh(Mesh* mesh)
{
    if (meshRenderer)
    {
        meshRenderer->SetMesh(mesh);
    }

    if (mesh)
    {
        mesh->Bind();
        {
            glBindBuffer(GL_ARRAY_BUFFER, PBO);

            glEnableVertexAttribArray(4 /* Instanced Particle Position */);
            glVertexAttribPointer(4 /* Instanced Particle Position */, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
            glVertexAttribDivisor(4 /* Instanced Particle Position */, 1);
        }
        mesh->Unbind();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void ParticleSystemComponent::CreateMeshRenderer()
{
    if (!meshRenderer)
    {
        meshRenderer = GetOwner()->AttachComponent<MeshRendererComponent>();
        meshRenderer->SetEnabled(false);
        meshRenderer->SetCastShadows(false);
        meshRenderer->SetDontCull(true);
    }
}

void ParticleSystemComponent::DestroyMeshRenderer()
{
    if (meshRenderer)
    {
        meshRenderer->Destroy();
        meshRenderer = nullptr;
    }
}

void ParticleSystemComponent::CreateVAOAndPBO()
{
    if (!VAO && !PBO)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &PBO);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, PBO);
        {
            glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * PARTICLE_BUFFER_SIZE, particleBuffer, GL_STATIC_DRAW);

            glBindVertexArray(VAO);
            {
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, PBO);
            }
            glBindVertexArray(0);
        }
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}

void ParticleSystemComponent::DeleteVAOAndPBO()
{
    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if (PBO)
    {
        glDeleteBuffers(1, &PBO);
        PBO = 0;
    }
}

void ParticleSystemComponent::Simulate(float deltaTime)
{
    if (computeShaderProgram && particleCount)
    {
        glBindVertexArray(VAO);
        {
            computeShaderProgram->Use();

            computeShaderProgram->SetUniform1fv(0, deltaTime);
            computeShaderProgram->SetUniform1i(1, particleCount);
            computeShaderProgram->SetUniform3fv(2, minRelativeEmitPosition);
            computeShaderProgram->SetUniform3fv(3, maxRelativeEmitPosition);
            computeShaderProgram->SetUniform3fv(4, minParticleVelocity);
            computeShaderProgram->SetUniform3fv(5, maxParticleVelocity);
            computeShaderProgram->SetUniform1fv(6, minParticleSize);
            computeShaderProgram->SetUniform1fv(7, maxParticleSize);
            computeShaderProgram->SetUniform1fv(8, minParticleLifetime);
            computeShaderProgram->SetUniform1fv(9, maxParticleLifetime);
            computeShaderProgram->SetUniform2fv(10, glm::vec2((float)rand() / RAND_MAX, (float)rand() / RAND_MAX));

            computeShaderProgram->DispatchCompute(particleCount / WORK_GROUP_SIZE + 1, 1, 1);

            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
        }
        glBindVertexArray(0);
    }
}
