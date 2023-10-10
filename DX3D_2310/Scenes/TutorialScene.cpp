#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
	Boxs.emplace_back(2, XMFLOAT3{ 0, 0, 0 });
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Update()
{
}

void TutorialScene::PreRneder()
{
}

void TutorialScene::Render()
{
	for (auto i = Boxs.begin(); i != Boxs.end(); i++)
		i->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
}
