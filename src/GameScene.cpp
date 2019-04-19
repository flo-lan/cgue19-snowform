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
}
