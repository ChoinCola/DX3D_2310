#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
    //ObjectTest = new Spher(4, 3);
    //sun = new Sun();
    //ealth = new Ealth({ 0, 0, 0 });
    //moon = new Moon(ealth->GetSelf());
    baseline = new BaseLine(10, 2000);

    XMVECTOR eye = XMVectorSet(3, 300, -300, 0);//Cam Pos
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

    //if (KEY->Press('W')) ObjectTest->Translate(Vector3(0, 0, 1) * DELTA);
    //if (KEY->Press('S')) ObjectTest->Translate(Vector3(0, 0, -1) * DELTA);
    //if (KEY->Press('A')) ObjectTest->Translate(Vector3(-1, 0, 0) * DELTA);
    //if (KEY->Press('D')) ObjectTest->Translate(Vector3(1, 0, 0) * DELTA);
    //if (KEY->Press('Q')) ObjectTest->Translate(Vector3(0, 1, 0) * DELTA);
    //if (KEY->Press('E')) ObjectTest->Translate(Vector3(0, -1, 0) * DELTA);

    //if (KEY->Press('T')) ObjectTest->Rotate(Vector3(0, 0, 1) * DELTA);
    //if (KEY->Press('G')) ObjectTest->Rotate(Vector3(0, 0, -1) * DELTA);
    //if (KEY->Press('F')) ObjectTest->Rotate(Vector3(-1, 0, 0) * DELTA);
    //if (KEY->Press('H')) ObjectTest->Rotate(Vector3(1, 0, 0) * DELTA);
    //if (KEY->Press('R')) ObjectTest->Rotate(Vector3(0, 1, 0) * DELTA);
    //if (KEY->Press('Y')) ObjectTest->Rotate(Vector3(0, -1, 0) * DELTA);

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
