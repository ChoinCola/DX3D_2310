#include "Framework.h"

TopViewGame::TopViewGame()
{
	player = new TopviewTraveler();
	map = new Map(player, Float2(10, 10));

	FireBallManager::Get();
	MonsterManager::Get();
	MonsterManager::Get()->SetTarget(player);
	CAM->SetTarget(player);
	CAM->TargetOptionLoad("Survival");
}

TopViewGame::~TopViewGame()
{
	delete player;
	delete map;


	FireBallManager::Delete();

	MonsterManager::Delete();
}

void TopViewGame::Update()
{
	player->Update();
	map->Update();

	FireBallManager::Get()->Update();
	MonsterManager::Get()->Update();
}

void TopViewGame::PreRender()
{
}

void TopViewGame::Render()
{
	player->Render();
	map->Render();

	FireBallManager::Get()->Render();
	MonsterManager::Get()->Render();
}

void TopViewGame::PostRender()
{
	player->PostRender();
	MonsterManager::Get()->PostRender();
}

void TopViewGame::GUIRender()
{
	//map->GUIRender();
}
