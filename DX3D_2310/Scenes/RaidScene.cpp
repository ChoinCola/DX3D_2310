#include "Framework.h"
#include "RaidScene.h"

RaidScene::RaidScene()
{
	traveler = new Traveler();
	terrain = new Terrain(L"Textures/HeightMaps/Miro.png", 1.0f, true);

	CAM->SetTarget(traveler);
	CAM->TargetOptionLoad("Raid");
}

RaidScene::~RaidScene()
{
	delete traveler;
	delete terrain;
}

void RaidScene::Update()
{
	traveler->Update();
}

void RaidScene::PreRender()
{
}

void RaidScene::Render()
{
	traveler->Render();
	terrain->Render();
}

void RaidScene::PostRender()
{
}

void RaidScene::GUIRender()
{
	traveler->GUIRender();
}
