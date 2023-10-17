#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
    robot = new Robot({0,0,0},{1,1,1});
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Update()
{

    if (KEY->Press('W')) robot->GetTransform()->Translate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('S')) robot->GetTransform()->Translate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('A')) robot->GetTransform()->Translate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('D')) robot->GetTransform()->Translate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('Q')) robot->GetTransform()->Translate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('E')) robot->GetTransform()->Translate(Vector3(0, -1, 0) * DELTA);

    if (KEY->Press('T')) robot->GetTransform()->Rotate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('G')) robot->GetTransform()->Rotate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('F')) robot->GetTransform()->Rotate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('H')) robot->GetTransform()->Rotate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('R')) robot->GetTransform()->Rotate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('Y')) robot->GetTransform()->Rotate(Vector3(0, -1, 0) * DELTA);

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
}
