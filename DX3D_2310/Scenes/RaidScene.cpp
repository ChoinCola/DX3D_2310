#include "Framework.h"
#include "RaidScene.h"

RaidScene::RaidScene()
{
	traveler = new Traveler();
	monster = new Monster();
	monster->SetLocalPosition(Vector3(10, 0, 10));
	monster->SetPlayerCollider(traveler);

	terrain = new Terrain(L"Textures/HeightMaps/Miro.png", 1.0f, true);

	CAM->SetTarget(traveler);
	CAM->TargetOptionLoad("Raid");
}

RaidScene::~RaidScene()
{
	delete monster;
	delete traveler;
	delete terrain;
}

void RaidScene::Update()
{
	traveler->Update();
	monster->Update();
}

void RaidScene::PreRender()
{
}

void RaidScene::Render()
{
	traveler->Render();
	//terrain->Render();
	monster->Render();
}

void RaidScene::PostRender()
{
}

void RaidScene::GUIRender()
{
	traveler->GUIRender();
	monster->GUIRender();
}
