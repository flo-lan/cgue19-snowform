#include "GameScene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ArcBallControllerComponent.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SimpleMaterial.h"
#include "AssetManager.h"
#include "StandardMaterial.h"
#include "MeshRendererComponent.h"
#include "Settings.h"
#include "Mesh.h"

GameScene::GameScene() :
    Scene::Scene("Game Scene")
{
}

GameScene::~GameScene()
{
}

void GameScene::OnLoad()
{
    /******************* Directional Light *******************/
    GameObject* directionalLight = CreateGameObject("Directional Light");

    TransformComponent* directionalLightTransformComponent = directionalLight->GetComponent<TransformComponent>();
    DirectionalLightComponent* directionalLightComponent = directionalLight->AttachComponent<DirectionalLightComponent>();
    directionalLightComponent->SetDirection(glm::vec3(0.f, -1.f, -1.f));
    directionalLightComponent->SetColor(glm::vec3(0.8f, 0.8f, 0.8f));

    /********************** Point Light **********************/
    GameObject* pointLight = CreateGameObject("Point Light");

    TransformComponent* pointLightTransformComponent = pointLight->GetComponent<TransformComponent>();
    pointLightTransformComponent->SetLocalPosition(0.f, 0.f, 0.f);
    PointLightComponent* pointLightComponent = pointLight->AttachComponent<PointLightComponent>();
    pointLightComponent->SetColor(glm::vec3(1.f, 1.f, 1.f));
    pointLightComponent->SetConstant(1.f);
    pointLightComponent->SetLinear(0.4f);
    pointLightComponent->SetQuadratic(0.1f);


    /************************* Sphere *************************/
    GameObject* sphere = CreateGameObject("Sphere");

    TransformComponent* sphereTransformComponent = sphere->GetComponent<TransformComponent>();
    sphereTransformComponent->SetLocalPosition(1.5f, -1.f, 0.f);

    StandardMaterial* sphereMaterial = sAssetManager.GetMaterial<StandardMaterial>("Sphere");
    sphereMaterial->SetDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    sphereMaterial->SetDiffuseTexture(sAssetManager.GetTexture2D("bricks_diffuse"));
    sphereMaterial->SetSpecularTexture(sAssetManager.GetTexture2D("bricks_specular"));
    sphereMaterial->SetAmbientReflectionConstant(0.1f);
    sphereMaterial->SetDiffuseReflectionConstant(0.7f);
    sphereMaterial->SetSpecularReflectionConstant(0.3f);
    sphereMaterial->SetShininess(8.f);

    MeshRendererComponent* sphereMeshRendererComponent = sphere->AttachComponent<MeshRendererComponent>();
    sphereMeshRendererComponent->SetMaterial(sphereMaterial);
    sphereMeshRendererComponent->SetMesh(sAssetManager.GetMesh("Sphere"));
    sphereMeshRendererComponent->AddLight(directionalLightComponent);
    sphereMeshRendererComponent->AddLight(pointLightComponent);

    /************************* Cylinder *************************/
    GameObject* cylinder = CreateGameObject("Cylinder");

    TransformComponent* cylinderTransformComponent = cylinder->GetComponent<TransformComponent>();
    cylinderTransformComponent->SetLocalPosition(-1.5f, -1.0f, 0.f);

    StandardMaterial* cylinderMaterial = sAssetManager.GetMaterial<StandardMaterial>("Cylinder");
    cylinderMaterial->SetDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    cylinderMaterial->SetDiffuseTexture(sAssetManager.GetTexture2D("bricks_diffuse"));
    cylinderMaterial->SetSpecularTexture(sAssetManager.GetTexture2D("bricks_specular"));
    cylinderMaterial->SetAmbientReflectionConstant(0.1f);
    cylinderMaterial->SetDiffuseReflectionConstant(0.7f);
    cylinderMaterial->SetSpecularReflectionConstant(0.3f);
    cylinderMaterial->SetShininess(8.f);

    MeshRendererComponent* cylinderMeshRendererComponent = cylinder->AttachComponent<MeshRendererComponent>();
    cylinderMeshRendererComponent->SetMaterial(cylinderMaterial);
    cylinderMeshRendererComponent->SetMesh(sAssetManager.GetMesh("Cylinder"));
    cylinderMeshRendererComponent->AddLight(directionalLightComponent);
    cylinderMeshRendererComponent->AddLight(pointLightComponent);
}
