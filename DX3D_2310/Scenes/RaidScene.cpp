#include "Framework.h"
#include "RaidScene.h"

RaidScene::RaidScene()
{
	traveler = new Traveler();
}

RaidScene::~RaidScene()
{
	delete traveler;
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
}

void RaidScene::PostRender()
{
}

void RaidScene::GUIRender()
{
	traveler->GUIRender();
}
