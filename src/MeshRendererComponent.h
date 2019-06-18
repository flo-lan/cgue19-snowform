#pragma once

#include "Component.h"
#include <GL/glew.h>
#include <vector>
#include <string>

class CameraComponent;
class TransformComponent;
class DirectionalLightComponent;
class PointLightComponent;
class SpotLightComponent;
class Material;
class Mesh;

class MeshRendererComponent : public Component
{
public:
    MeshRendererComponent(GameObject* owner);
    virtual ~MeshRendererComponent();

    virtual void OnStart();

    bool IsEnabled() const { return enabled; }

    void Render(CameraComponent* camera) { Render(camera, material); }
    void Render(CameraComponent* camera, Material* material);

    void SetEnabled(bool value) { enabled = value; }
    void SetCastShadows(bool castShadows) { this->castShadows = castShadows; }
    void SetMaterial(Material* value) { material = value; }
    void SetMesh(Mesh* value);
    void SetInstanceCount(GLsizei instanceCount) { this->instanceCount = instanceCount; }

    void AddInitialLight(std::string const& gameObjectId); // ToDo: Should be handled inside MeshRendererComponentFactory

    void AddLight(DirectionalLightComponent* light);
    void AddLight(PointLightComponent* light);
    void AddLight(SpotLightComponent* light);

    void RemoveLight(DirectionalLightComponent* light);
    void RemoveLight(PointLightComponent* light);
    void RemoveLight(SpotLightComponent* light);

    bool IsCastShadowsEnabled() const { return castShadows; }
    TransformComponent* GetTransform() const { return transform; }
    Material* GetMaterial() const { return material; }
    std::vector<DirectionalLightComponent*> const& GetDirectionalLights() { return directionalLights; }
    std::vector<PointLightComponent*> const& GetPointLights() { return pointLights; }
    std::vector<SpotLightComponent*> const& GetSpotLights() { return spotLights; }

private:
    bool enabled;
    bool castShadows;
    TransformComponent* transform;
    Material* material;
    Mesh* mesh;
    GLsizei instanceCount;
    std::vector<DirectionalLightComponent*> directionalLights;
    std::vector<PointLightComponent*> pointLights;
    std::vector<SpotLightComponent*> spotLights;
    std::vector<std::string> initialLights; // ToDo: Should be handled inside MeshRendererComponentFactory
};
