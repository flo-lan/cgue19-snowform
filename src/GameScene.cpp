#include "GameScene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ArcBallControllerComponent.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SimpleMaterial.h"
#include "AssetManager.h"
#include "PhongGouraudMaterial.h"
#include "MeshRendererComponent.h"
#include "INIReader.h"
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
    // init reader for ini files
    INIReader reader("assets/settings.ini");

    // load values from ini file
    // first param: section [window], second param: property name, third param: default value
    int width = reader.GetInteger("window", "width", 800);
    int height = reader.GetInteger("window", "height", 800);

    /************************ Focus Point ***********************/
    GameObject* focusPoint = CreateGameObject("Focus Point");

    TransformComponent* focusPointTransformComponent = focusPoint->AttachComponent<TransformComponent>();
    ArcBallControllerComponent* arcBallControllerComponent = focusPoint->AttachComponent<ArcBallControllerComponent>();

    /************************ Main Camera ***********************/
    GameObject* mainCamera = CreateGameObject("Main Camera");

    TransformComponent* mainCameraTransformComponent = mainCamera->AttachComponent<TransformComponent>();
    // Add main camera transform to focus point transform
    focusPointTransformComponent->AddChild(mainCameraTransformComponent);
    mainCameraTransformComponent->SetLocalPosition(0.f, 0.f, 6.f);

    CameraComponent* mainCameraComponent = mainCamera->AttachComponent<CameraComponent>();
    mainCameraComponent->SetFov((float)glm::radians(reader.GetReal("camera", "fov", 60.f)));
    mainCameraComponent->SetAspectRatio((float)width / height);
    mainCameraComponent->SetNearPlane((float)reader.GetReal("camera", "near", 0.1f));
    mainCameraComponent->SetFarPlane((float)reader.GetReal("camera", "far", 100.f));

    /******************* Directional Light *******************/
    GameObject* directionalLight = CreateGameObject("Directional Light");

    TransformComponent* directionalLightTransformComponent = directionalLight->AttachComponent<TransformComponent>();
    DirectionalLightComponent* directionalLightComponent = directionalLight->AttachComponent<DirectionalLightComponent>();
    directionalLightComponent->SetDirection(glm::vec3(0.f, -1.f, -1.f));
    directionalLightComponent->SetColor(glm::vec3(0.8f, 0.8f, 0.8f));

    /********************** Point Light **********************/
    GameObject* pointLight = CreateGameObject("Point Light");

    TransformComponent* pointLightTransformComponent = pointLight->AttachComponent<TransformComponent>();
    pointLightTransformComponent->SetLocalPosition(0.f, 0.f, 0.f);
    PointLightComponent* pointLightComponent = pointLight->AttachComponent<PointLightComponent>();
    pointLightComponent->SetColor(glm::vec3(1.f, 1.f, 1.f));
    pointLightComponent->SetConstant(1.f);
    pointLightComponent->SetLinear(0.4f);
    pointLightComponent->SetQuadratic(0.1f);

    /************************* Torus *************************/
    GameObject* torus = CreateGameObject("Torus");

    TransformComponent* torusTransformComponent = torus->AttachComponent<TransformComponent>();
    torusTransformComponent->SetLocalRotation(glm::radians(90.f), 0.f, 0.f);
    torusTransformComponent->SetLocalScale(1.f, 1.f, 0.6f);

    SimpleMaterial* torusMaterial = sAssetManager.GetMaterial<SimpleMaterial>("Torus");
    torusMaterial->SetColor(glm::vec3(1.f, 0.f, 1.f)); // Magenta

    //MeshRendererComponent* torusMeshRendererComponent = torus->AttachComponent<MeshRendererComponent>();
    //torusMeshRendererComponent->SetCamera(mainCameraComponent);
    //torusMeshRendererComponent->SetMaterial(torusMaterial);
    //torusMeshRendererComponent->SetMesh(sAssetManager.GetMesh("Torus"));


    /************************* Sphere *************************/
    GameObject* sphere = CreateGameObject("Sphere");

    TransformComponent* sphereTransformComponent = sphere->AttachComponent<TransformComponent>();
    sphereTransformComponent->SetLocalPosition(1.5f, -1.f, 0.f);

    PhongGouraudMaterial* sphereMaterial = sAssetManager.GetMaterial<PhongGouraudMaterial>("Sphere");
    sphereMaterial->SetDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    sphereMaterial->SetDiffuseTexture(sAssetManager.GetTexture2D("bricks_diffuse"));
    sphereMaterial->SetSpecularTexture(sAssetManager.GetTexture2D("bricks_specular"));
    sphereMaterial->SetAmbientReflectionConstant(0.1f);
    sphereMaterial->SetDiffuseReflectionConstant(0.7f);
    sphereMaterial->SetSpecularReflectionConstant(0.3f);
    sphereMaterial->SetShininess(8.f);

    MeshRendererComponent* sphereMeshRendererComponent = sphere->AttachComponent<MeshRendererComponent>();
    sphereMeshRendererComponent->SetCamera(mainCameraComponent);
    sphereMeshRendererComponent->SetMaterial(sphereMaterial);
    sphereMeshRendererComponent->SetMesh(sAssetManager.GetMesh("Sphere"));
    sphereMeshRendererComponent->AddLight(directionalLightComponent);
    sphereMeshRendererComponent->AddLight(pointLightComponent);

    /************************* Cylinder *************************/
    GameObject* cylinder = CreateGameObject("Cylinder");

    TransformComponent* cylinderTransformComponent = cylinder->AttachComponent<TransformComponent>();
    cylinderTransformComponent->SetLocalPosition(-1.5f, -1.0f, 0.f);

    PhongGouraudMaterial* cylinderMaterial = sAssetManager.GetMaterial<PhongGouraudMaterial>("Cylinder");
    cylinderMaterial->SetDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    cylinderMaterial->SetDiffuseTexture(sAssetManager.GetTexture2D("bricks_diffuse"));
    cylinderMaterial->SetSpecularTexture(sAssetManager.GetTexture2D("bricks_specular"));
    cylinderMaterial->SetAmbientReflectionConstant(0.1f);
    cylinderMaterial->SetDiffuseReflectionConstant(0.7f);
    cylinderMaterial->SetSpecularReflectionConstant(0.3f);
    cylinderMaterial->SetShininess(8.f);

    MeshRendererComponent* cylinderMeshRendererComponent = cylinder->AttachComponent<MeshRendererComponent>();
    cylinderMeshRendererComponent->SetCamera(mainCameraComponent);
    cylinderMeshRendererComponent->SetMaterial(cylinderMaterial);
    cylinderMeshRendererComponent->SetMesh(sAssetManager.GetMesh("Cylinder"));
    cylinderMeshRendererComponent->AddLight(directionalLightComponent);
    cylinderMeshRendererComponent->AddLight(pointLightComponent);

    /************************* Cube *************************/
    GameObject* cube = CreateGameObject("Cube");

    TransformComponent* cubeTransformComponent = cube->AttachComponent<TransformComponent>();
    cubeTransformComponent->SetLocalPosition(0.0f, 1.5f, 0.f);

    PhongGouraudMaterial* cubeMaterial = sAssetManager.GetMaterial<PhongGouraudMaterial>("Cube");
    cubeMaterial->SetDiffuseColor(glm::vec3(1.f, 1.f, 1.f));
    cubeMaterial->SetDiffuseTexture(sAssetManager.GetTexture2D("wood_diffuse"));
    cubeMaterial->SetAmbientReflectionConstant(0.1f);
    cubeMaterial->SetDiffuseReflectionConstant(0.7f);
    cubeMaterial->SetSpecularReflectionConstant(0.1f);
    cubeMaterial->SetShininess(2.f);

    MeshRendererComponent* cubeMeshRendererComponent = cube->AttachComponent<MeshRendererComponent>();
    cubeMeshRendererComponent->SetCamera(mainCameraComponent);
    cubeMeshRendererComponent->SetMaterial(cubeMaterial);
    cubeMeshRendererComponent->SetMesh(sAssetManager.GetMesh("Cube"));
    cubeMeshRendererComponent->AddLight(directionalLightComponent);
    cubeMeshRendererComponent->AddLight(pointLightComponent);
}
