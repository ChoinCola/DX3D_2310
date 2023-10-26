#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
    robot = new Robot({ 0, 0, 0 }, 1);
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Update()
{

    robot->UpdateWorld();
    robot->Update();
}

void TutorialScene::PreRender()
{
}

void TutorialScene::Render()
{
    robot->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
    robot->GUIRender();
}
