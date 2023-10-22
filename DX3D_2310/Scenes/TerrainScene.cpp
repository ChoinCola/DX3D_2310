#include "Framework.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
}

TerrainScene::~TerrainScene()
{
}

void TerrainScene::Update()
{
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
	terrain->GUIRender();
}
