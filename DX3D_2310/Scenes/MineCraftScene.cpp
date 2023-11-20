#include "Framework.h"
#include "MineCraftScene.h"

MinceCraftScene::MinceCraftScene()
{
	BlockDataManager::Get()->LoadTable();
	BlockManager::Get()->CreateBlocks(10, 3, 10);
	Environment::Get()->GetLightBuffer()->GetData()->lights[0].isActive = true;

	player = new Steve();
	seller = new Seller();
}

MinceCraftScene::~MinceCraftScene()
{
	BlockManager::Delete();
	BlockDataManager::Delete();

	delete player;
	delete seller;
}

void MinceCraftScene::Update()
{
	player->Update();
	seller->Update();
	BlockManager::Get()->Update();
	MouseBag::Get()->Update();
}

void MinceCraftScene::PreRender()
{
}

void MinceCraftScene::Render()
{
	player->Render();
	BlockManager::Get()->Redner();
	seller->Render();
}

void MinceCraftScene::PostRender()
{
	player->PostRender();
	seller->PostRender();
	MouseBag::Get()->Render();
}

void MinceCraftScene::GUIRender()
{
	//BlockManager::Get()->GUIRender();
	player->GUIRender();
	seller->GUIRender();
}