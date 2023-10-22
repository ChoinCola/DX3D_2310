#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
    //cube = new Cube();
    ground = new Ground(10,1);
    //cube->GetMetrial()->SetDiffuseMap(L"Textures/Landscape/box.png");
    ground->GetMetrial()->SetDiffuseMap(L"Textures/Landscape/Bricks.png");

    ground->SetTag("Ground1");
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Update()
{

    if (KEY->Press('W')) ground->Translate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('S')) ground->Translate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('A')) ground->Translate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('D')) ground->Translate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('Q')) ground->Translate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('E')) ground->Translate(Vector3(0, -1, 0) * DELTA);

    if (KEY->Press('T')) ground->Rotate(Vector3(0, 0, 1) * DELTA);
    if (KEY->Press('G')) ground->Rotate(Vector3(0, 0, -1) * DELTA);
    if (KEY->Press('F')) ground->Rotate(Vector3(-1, 0, 0) * DELTA);
    if (KEY->Press('H')) ground->Rotate(Vector3(1, 0, 0) * DELTA);
    if (KEY->Press('R')) ground->Rotate(Vector3(0, 1, 0) * DELTA);
    if (KEY->Press('Y')) ground->Rotate(Vector3(0, -1, 0) * DELTA);

    //cube->UpdateWorld();
    ground->UpdateWorld();
    ground->DivideUpdate(sliderValue);
}

void TutorialScene::PreRender()
{
}

void TutorialScene::Render()
{
    //cube->Render();
    ground->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
    ground->GUIRender();

    if (ImGui::Button("Save"))
        DIALOG->OpenDialog("Save", "Save", ".png, .jpg, .tga", ".");
    
    if (ImGui::Button("Slice"))
        showsliceUI = true;

    if (ImGui::Button("SHOWFRAME")) {
        ground->GetMetrial()->SetShader(L"WhiteLINE.hlsl");

        D3D11_RASTERIZER_DESC Desc;
        ZeroMemory(&Desc, sizeof(Desc));
        Desc.FillMode = D3D11_FILL_WIREFRAME; // 내부를 채우지 않도록 WIREFRAME 모드로 설정
        Desc.CullMode = D3D11_CULL_NONE;

        ID3D11RasterizerState* wireframe;
        DEVICE->CreateRasterizerState(&Desc, &wireframe);
        DC->RSSetState(wireframe);
    }

    if (ImGui::Button("SHOWRACE")) {
        ground->GetMetrial()->SetShader(L"Tutorial.hlsl");
        DC->RSSetState(nullptr);
    }

    if (DIALOG->Display("Save"))
    {
        if (DIALOG->IsOk()) {
            string file = DIALOG->GetFilePathName();
            ground->GetMetrial()->SetDiffuseMap(ToWString(file));
            DIALOG->Close();
        }
        else
            DIALOG->Close();
    }

    if (showsliceUI)
        ImGui::OpenPopup("SliceUI");

    if (ImGui::BeginPopup("SliceUI")) {
        ImGui::Text("UI");
        ImGui::SliderInt("Slice", (int*)&sliderValue, 0, 6);
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            showsliceUI = false;
        }
        ImGui::EndPopup();
    }
}
