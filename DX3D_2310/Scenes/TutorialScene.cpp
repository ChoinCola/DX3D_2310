#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
    Cube* defc = new Cube();
	cubes.emplace_back(defc);

    XMVECTOR eye = XMVectorSet(3, 30, -30, 0);//Cam Pos
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
    for (Cube* cube : cubes)
        cube->Update();
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
