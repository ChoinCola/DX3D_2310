#include "Framework.h"

TopViewGame::TopViewGame()
{
	player = new TopPlayer();
	map = new Map(player, Float2(10, 10));
	CAM->SetTarget(player);
}

TopViewGame::~TopViewGame()
{
	delete player;
	delete map;
}

void TopViewGame::Update()
{
	player->Update();
	map->Update();
}

void TopViewGame::PreRender()
{
}

void TopViewGame::Render()
{
	player->Render();
	map->Render();
}

void TopViewGame::PostRender()
{
}

void TopViewGame::GUIRender()
{
}
