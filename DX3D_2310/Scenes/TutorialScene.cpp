#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
    Cube* defc = new Cube();
	cubes.emplace_back(defc);
    cubes.push_back(new Cube());

    //cubes.back()->SetPivot(Vector3(0.5f, 0.5f, 0.5f));

    cubes.back()->SetLocalPosition({ 2, 0, 0 });
    cubes[1]->SetParent(cubes[0]);

    XMVECTOR eye = XMVectorSet(3, 3, -3, 0);//Cam Pos
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
}

TutorialScene::~TutorialScene()
{
    delete viewBuffer;
    delete projectionBuffer;
}

void TutorialScene::Update()
{
    if (KEY->Press('W'))
        cubes[0]->Translate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('S'))
        cubes[0]->Translate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('A'))
        cubes[0]->Translate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('D'))
        cubes[0]->Translate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('Q'))
        cubes[0]->Translate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('E'))
        cubes[0]->Translate(Vector3(0, -1, 0) * DELTA);

    if (KEY->Press('T'))
        cubes[0]->Rotate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('G'))
        cubes[0]->Rotate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('F'))
        cubes[0]->Rotate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('H'))
        cubes[0]->Rotate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('R'))
        cubes[0]->Rotate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('Y'))
        cubes[0]->Rotate(Vector3(0, -1, 0) * DELTA);

    for (Cube* cube : cubes)
        cube->UpdateWorld();
}

void TutorialScene::PreRneder()
{
}

void TutorialScene::Render()
{
    for (Cube* cube : cubes)
        cube->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
}
