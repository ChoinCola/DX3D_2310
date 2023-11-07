#include "Framework.h"
#include "MineCraftScene.h"

MinceCraftScene::MinceCraftScene()
{
	BlockManager::Get()->CreateBlocks(10, 3, 10);
}

MinceCraftScene::~MinceCraftScene()
{
	BlockManager::Delete();
}

void MinceCraftScene::Update()
{
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
}

void MinceCraftScene::GUIRender()
{
}
