#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
    cube = new Cube();

    cube->GetMetrial()->SetDiffuseMap(L"Textures/Landscape/box.png");
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Update()
{

    if (KEY->Press('W')) cube->Translate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('S')) cube->Translate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('A')) cube->Translate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('D')) cube->Translate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('Q')) cube->Translate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('E')) cube->Translate(Vector3(0, -1, 0) * DELTA);

    if (KEY->Press('T')) cube->Rotate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('G')) cube->Rotate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('F')) cube->Rotate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('H')) cube->Rotate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('R')) cube->Rotate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('Y')) cube->Rotate(Vector3(0, -1, 0) * DELTA);

    cube->UpdateWorld();
}

void TutorialScene::PreRender()
{
}

void TutorialScene::Render()
{
    cube->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
    if (ImGui::Button("Save"))
        DIALOG->OpenDialog("Save", "Save", ".png, .jpg, .tga", ".");

    if (DIALOG->Display("Save"))
    {
        if (DIALOG->IsOk()) {
            string file = DIALOG->GetFilePathName();
            cube->GetMetrial()->SetDiffuseMap(wstring(file.begin(), file.end()));
            DIALOG->Close();
        }
        else
            DIALOG->Close();
    }
}
