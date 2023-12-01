#include "Framework.h"
#include "RaidScene.h"

RaidScene::RaidScene()
{
	traveler = new TopviewTraveler();
	//monster = new Monster();
	//monster->SetLocalPosition(Vector3(20, 2, 20));
	//monster->SetPlayerCollider(traveler);

	//terrain = new Terrain(L"Textures/HeightMaps/Miro.png", 1.0f, true);
	FireBallManager::Get();
	MonsterManager::Get();
	MonsterManager::Get()->SetTarget(traveler);
	CAM->SetTarget(traveler);
	CAM->TargetOptionLoad("Survival");
}

RaidScene::~RaidScene()
{
	//delete monster;
	delete traveler;
	//delete terrain;

	FireBallManager::Delete();

	MonsterManager::Delete();
}

void RaidScene::Update()
{
	traveler->Update();
	//monster->Update();
	FireBallManager::Get()->Update();
	MonsterManager::Get()->Update();
}

void RaidScene::PreRender()
{
}

void RaidScene::Render()
{
	traveler->Render();
	//terrain->Render();
	//monster->Render();
	FireBallManager::Get()->Render();
	MonsterManager::Get()->Render();
}

void RaidScene::PostRender()
{
	//traveler->PostRender();
}

void RaidScene::GUIRender()
{
	traveler->GUIRender();
	//monster->GUIRender();
	//FireBallManager::Get()->GUIRender();
	MonsterManager::Get()->GUIRender();
}
