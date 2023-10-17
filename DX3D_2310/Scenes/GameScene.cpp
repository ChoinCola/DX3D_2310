#include "Framework.h"

GameScene::GameScene()
{

	MovePoint = new GameBox();
	MovePoint->SetImage(L"Textures/Localgame/MovePoint");
	MovePoint->SetLocalPosition({ MAX_SIZE-1, 0, 0 });

	for (int i = 0; i < pow(MAX_SIZE,2)-1; i++) {
		GameBox* defbox = new GameBox(MAX_SIZE, i, { 1,1,1 }, {});
		GameBord.push_back();
	}
}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
}

void GameScene::PreRender()
{
}

void GameScene::Render()
{
}

void GameScene::PostRender()
{
}

void GameScene::GUIRender()
{
}

void GameScene::ChackMove(GameBox* nowBox)
{
}

void GameScene::BoxUP(GameBox* nowBox)
{
}

void GameScene::BoxDOWN(GameBox* nowBox)
{
}

void GameScene::BoxLEFT(GameBox* nowBox)
{
}

void GameScene::BoxRIGHT(GameBox* nowBox)
{
}

void GameScene::BoxMIX()
{
}
