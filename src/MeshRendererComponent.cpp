#include "MeshRendererComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "Material.h"
#include "Mesh.h"
#include "Utils.h"
#include <algorithm>

MeshRendererComponent::MeshRendererComponent(GameObject* owner) :
    Component::Component(owner),
    enabled(true),
    camera(nullptr),
    transform(owner->GetComponent<TransformComponent>()),
    material(nullptr),
    mesh(nullptr),
    VAO(0),
    VBO(0),
    EBO(0)
{
    fprintf(stdout, "Attached mesh renderer component to game object '%s'!\n", GetOwner()->GetName().c_str());

    GetOwner()->InsertMeshRendererComponent(this);
}

MeshRendererComponent::~MeshRendererComponent()
{
    fprintf(stdout, "Deleted mesh renderer component from game object '%s'!\n", GetOwner()->GetName().c_str());

    for (auto itr = directionalLights.begin(); itr != directionalLights.end(); itr++)
    {
        (*itr)->RemoveAffectedMeshRendererComponent(this);
    }

    for (auto itr = pointLights.begin(); itr != pointLights.end(); itr++)
    {
        (*itr)->RemoveAffectedMeshRendererComponent(this);
    }

    for (auto itr = spotLights.begin(); itr != spotLights.end(); itr++)
    {
        (*itr)->RemoveAffectedMeshRendererComponent(this);
    }

    directionalLights.clear();
    pointLights.clear();
    spotLights.clear();

    GetOwner()->RemoveMeshRendererComponent(this);

    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
    }

    if (VBO)
    {
        glDeleteBuffers(1, &VBO);
    }

    if (EBO)
    {
        glDeleteBuffers(1, &EBO);
    }
}

void MeshRendererComponent::OnStart()
{
    // ToDo: Should be handled inside MeshRendererComponentFactory,
    // but must be processed _after_ all light game objects are de-
    // serialized, so the light component can be found inside scene.
    for (auto itr = initialLights.begin(); itr != initialLights.end(); ++itr)
    {
        GameObject* light = GetOwner()->GetScene()->GetGameObjectById(*itr);

        if (light == nullptr)
        {
            fprintf(stderr, "Could not find light game object with id '%s' for mesh renderer component of game object '%s'!\n", (*itr).c_str(), GetOwner()->GetName().c_str());
            continue;
        }

        if (DirectionalLightComponent* lightComponent = light->GetComponent<DirectionalLightComponent>())
        {
            AddLight(lightComponent);
        }

        if (PointLightComponent* lightComponent = light->GetComponent<PointLightComponent>())
        {
            AddLight(lightComponent);
        }

        if (SpotLightComponent* lightComponent = light->GetComponent<SpotLightComponent>())
        {
            AddLight(lightComponent);
        }
    }

    initialLights.clear();
}

void MeshRendererComponent::Render()
{
    if (material && mesh && enabled)
    {
        material->Use();
        material->SetUniforms(this);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh->Indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }
}

void MeshRendererComponent::SetMesh(Mesh* value)
{
    // Delete mesh from GPU
    if (mesh)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        VBO = 0;
        VAO = 0;
        EBO = 0;
    }

    mesh = value;

    // Upload mesh to GPU
    if (mesh)
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->Vertices.size() * sizeof(Vertex), &mesh->Vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->Indices.size() * sizeof(uint32_t), &mesh->Indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0 /* Vertex Position */);
        glVertexAttribPointer(0 /* Vertex Position */, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1 /* Vertex Normal */);
        glVertexAttribPointer(1 /* Vertex Normal */, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2 /* Vertex UV */);
        glVertexAttribPointer(2 /* Vertex UV */, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));

        glEnableVertexAttribArray(3 /* Vertex Color */);
        glVertexAttribPointer(3 /* Vertex Color */, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void MeshRendererComponent::AddInitialLight(std::string const & gameObjectId)
{
    initialLights.push_back(gameObjectId);
}

void MeshRendererComponent::AddLight(DirectionalLightComponent* light)
{
    if (directionalLights.size() >= MAX_DIRECTIONAL_LIGHT_COUNT)
    {
        fprintf(stderr, "Could not add directional light to mesh renderer component of game object '%s', because max light count is reached!\n", GetOwner()->GetName().c_str());
        return;
    }

    if (light)
    {
        light->InsertAffectedMeshRendererComponent(this);
        directionalLights.push_back(light);
    }
}

void MeshRendererComponent::AddLight(PointLightComponent* light)
{
    if (pointLights.size() >= MAX_POINT_LIGHT_COUNT)
    {
        fprintf(stderr, "Could not add point light to mesh renderer component of game object '%s', because max light count is reached!\n", GetOwner()->GetName().c_str());
        return;
    }

    if (light)
    {
        light->InsertAffectedMeshRendererComponent(this);
        pointLights.push_back(light);
    }
}

void MeshRendererComponent::AddLight(SpotLightComponent* light)
{
    if (pointLights.size() >= MAX_SPOT_LIGHT_COUNT)
    {
        fprintf(stderr, "Could not add spot light to mesh renderer component of game object '%s', because max light count is reached!\n", GetOwner()->GetName().c_str());
        return;
    }

    if (light)
    {
        light->InsertAffectedMeshRendererComponent(this);
        spotLights.push_back(light);
    }
}

void MeshRendererComponent::RemoveLight(DirectionalLightComponent* light)
{
    if (light)
    {
        light->RemoveAffectedMeshRendererComponent(this);
        directionalLights.erase(std::remove(directionalLights.begin(), directionalLights.end(), light), directionalLights.end());
    }
}

void MeshRendererComponent::RemoveLight(PointLightComponent* light)
{
    if (light)
    {
        light->RemoveAffectedMeshRendererComponent(this);
        pointLights.erase(std::remove(pointLights.begin(), pointLights.end(), light), pointLights.end());
    }
}

void MeshRendererComponent::RemoveLight(SpotLightComponent* light)
{
    if (light)
    {
        light->RemoveAffectedMeshRendererComponent(this);
        spotLights.erase(std::remove(spotLights.begin(), spotLights.end(), light), spotLights.end());
    }
}

CameraComponent* MeshRendererComponent::GetCamera() const
{
    return camera ? camera : CameraComponent::GetMainCameraComponent();
}
