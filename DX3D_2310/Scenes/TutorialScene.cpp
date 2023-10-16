#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
    //ObjectTest = new Spher(0.5,3);
    //sun = new Sun();
    //ealth = new Ealth({ 0, 0, 0 });
    //moon = new Moon(ealth->GetSelf());
    baseline = new BaseLine(1, 2000);
    //cube = new Cube({1, 1, 1},{1, 0, 0});
    robot = new Robot({0, 0, 0}, {1, 1, 1});
    XMVECTOR eye = XMVectorSet(3, 3, -30, 0);//Cam Pos
    XMVECTOR focus = XMVectorSet(0, 0, 0, 0);//Cam Look at Pos
    XMVECTOR up = XMVectorSet(0, 1, 0, 0);//Cam Up Vector

    Matrix view = XMMatrixLookAtLH(eye, focus, up);

    Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.0f);

    viewBuffer = new MatrixBuffer();
    projectionBuffer = new MatrixBuffer();

    viewBuffer->Set(view);
    viewBuffer->SetVS(1);

    projectionBuffer->Set(projection);
    projectionBuffer->SetVS(2);
    baseline->UpdateWorld();
}

TutorialScene::~TutorialScene()
{
    delete viewBuffer;
    delete projectionBuffer;
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

    //cube->UpdateWorld();
    //ObjectTest->UpdateWorld();
    //ealth->Update();
    //moon->Update();
    //sun->Update();

}

void TutorialScene::PreRneder()
{
}

void TutorialScene::Render()
{
    //for (Cube* cube : cubes)
    //    cube->Render();
    //ObjectTest->Render();

    baseline->Render();
    robot->Render();

    //cube->Render();
//    moon->Render();
//    ealth->Render();
//
//    sun->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
}
