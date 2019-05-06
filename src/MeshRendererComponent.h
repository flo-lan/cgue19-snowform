#pragma once

#include "Component.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

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
    virtual void Render();

    void SetCamera(CameraComponent* value) { camera = value; }
    void SetMaterial(Material* value) { material = value; }
    void SetMesh(Mesh* value);

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
};
