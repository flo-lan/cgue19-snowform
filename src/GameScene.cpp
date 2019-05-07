#include "GameScene.h"
#include <iostream>

GameScene::GameScene() :
    Scene::Scene("Game Scene"),
    coinCount(0),
    collectedCoinCount(0)
{
}

GameScene::~GameScene()
{
}

void GameScene::OnLoad()
{
}

void GameScene::OnGameWon()
{
}

void GameScene::OnGameLost()
{

}
