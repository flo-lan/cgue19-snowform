#pragma once

#include "Component.h"
#include <GL\glew.h>
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

    void Render();

    void SetEnabled(bool value) { enabled = value; }
    void SetCamera(CameraComponent* value) { camera = value; }
    void SetMaterial(Material* value) { material = value; }
    void SetMesh(Mesh* value);

    void AddInitialLight(std::string const& gameObjectId); // ToDo: Should be handled inside MeshRendererComponentFactory

    void AddLight(DirectionalLightComponent* light);
    void AddLight(PointLightComponent* light);
    void AddLight(SpotLightComponent* light);

    CameraComponent* GetCamera() const;
    TransformComponent* GetTransform() const { return transform; }
    Material* GetMaterial() const { return material; }
    std::vector<DirectionalLightComponent*> const& GetDirectionalLights() { return directionalLights; }
    std::vector<PointLightComponent*> const& GetPointLights() { return pointLights; }
    std::vector<SpotLightComponent*> const& GetSpotLights() { return spotLights; }

private:
    bool enabled;
    CameraComponent* camera;
    TransformComponent* transform;
    Material* material;
    Mesh* mesh;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    std::vector<DirectionalLightComponent*> directionalLights;
    std::vector<PointLightComponent*> pointLights;
    std::vector<SpotLightComponent*> spotLights;
    std::vector<std::string> initialLights; // ToDo: Should be handled inside MeshRendererComponentFactory
};
