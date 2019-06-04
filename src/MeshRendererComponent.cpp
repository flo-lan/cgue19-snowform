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
    castShadows(true),
    transform(owner->GetComponent<TransformComponent>()),
    material(nullptr),
    mesh(nullptr),
    instanceCount(0)
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

void MeshRendererComponent::Render(CameraComponent* camera, Material* material)
{
    if (camera && material && mesh && enabled)
    {
        material->Use();
        material->SetUniforms(camera, this);

        mesh->Bind();

        if (instanceCount > 0)
        {
            glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)mesh->GetIndexCount(), GL_UNSIGNED_INT, (void*)0, instanceCount);
        }
        else
        {
            glDrawElements(GL_TRIANGLES, (GLsizei)mesh->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
        }

        mesh->Unbind();
    }
}

void MeshRendererComponent::SetMesh(Mesh* value)
{
    mesh = value;

    if (mesh && !mesh->IsUploaded())
    {
        mesh->Upload();
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
