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
    if (KEY->Press('W')) robot->Translate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('S')) robot->Translate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('A')) robot->Translate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('D')) robot->Translate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('Q')) robot->Translate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('E')) robot->Translate(Vector3(0, -1, 0) * DELTA);

    if (KEY->Press('T')) robot->Rotate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('G')) robot->Rotate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('F')) robot->Rotate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('H')) robot->Rotate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('R')) robot->Rotate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('Y')) robot->Rotate(Vector3(0, -1, 0) * DELTA);

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
