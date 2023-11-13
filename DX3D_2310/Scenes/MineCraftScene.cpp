#include "Framework.h"
#include "MineCraftScene.h"

MinceCraftScene::MinceCraftScene()
{
	BlockDataManager::Get()->LoadTable();
	BlockManager::Get()->CreateBlocks(10, 3, 10);
	Environment::Get()->GetLightBuffer()->GetData()->lights[0].isActive = true;

	player = new Steve();
}

MinceCraftScene::~MinceCraftScene()
{
	BlockManager::Delete();
	BlockDataManager::Delete();

	delete player;
}

void MinceCraftScene::Update()
{
	player->Update();
	BlockManager::Get()->Update();
}

void MinceCraftScene::PreRender()
{
}

void MinceCraftScene::Render()
{
	BlockManager::Get()->Redner();
}

void MinceCraftScene::PostRender()
{
	player->PostRender();
}

void MinceCraftScene::GUIRender()
{
	BlockManager::Get()->GUIRender();
	player->GUIRender();
}