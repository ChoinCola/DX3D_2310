#include "Framework.h"
#include "GameManager.h"


GameManager::GameManager()
{
    Create();


    SceneManager::Get()->Create("Grid", new GridScene());
    //SceneManager::Get()->Create("Start", new RenderTargetScene());
    //SceneManager::Get()->Create("Start", new OutLineScene());
    //SceneManager::Get()->Create("Start", new SubScene());
    //SceneManager::Get()->Create("Start", new TerrainEditScene());
    //SceneManager::Get()->Create("Start", new ModelRenderScene());
    //SceneManager::Get()->Create("Start", new AnimationScene());
    //SceneManager::Get()->Create("Start", new AStarScene());
    SceneManager::Get()->Create("Start", new InstancingScene());
    //SceneManager::Get()->Create("Start", new TerrainScene());
    //SceneManager::Get()->Create("Start", new ModelInstancingScene());
    //SceneManager::Get()->Create("Topview", new TopViewGame());

    SceneManager::Get()->Add("Grid");
    SceneManager::Get()->Add("Start");
    //SceneManager::Get()->Add("Topview");
}

GameManager::~GameManager()
{
    Delete();
}

void GameManager::Update()
{
    KEY->Update();
    Mouse::Get()->Update();
    Timer::Get()->Update();
    Environment::Get()->Update();

    SceneManager::Get()->Update();
    UIRenderMaster::Get()->Update();
}

void GameManager::Render()
{
    DepthBuffer::Get()->SetPS(3);
    SceneManager::Get()->PreRender();

    Device::Get()->Clear();

    Environment::Get()->Set();
    Environment::Get()->SetViewport();

    SceneManager::Get()->Render();

    Font::Get()->GetDC()->BeginDraw(); // Ŭ����. ��Ʈ����ϱ� ���� �����۾�.

    Environment::Get()->SetPost();
    SceneManager::Get()->PostRender();
    UIRenderMaster::Get()->Render();
    UIRenderMaster::Get()->PostRender();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    Font::Get()->SetColor("white");
    Font::Get()->SetStyle("Default");
    Font::Get()->RenderText(fps, {100, 20});


    Environment::Get()->GUIRender();
    SceneManager::Get()->GUIRender();
    UIRenderMaster::Get()->GUIRender();

    Font::Get()->GetDC()->EndDraw(); // ��Ʈ��� ����.

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    Device::Get()->Present();
}

void GameManager::Create()
{
    Device::Get();
    Keyboard::Get();
    Timer::Get();
    Environment::Get();
    SceneManager::Get();
    Mouse::Get();
    Font::Get();
    UIRenderMaster::Get();
    Font::Get()->AddColor("white", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"��� ������10����ü");

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
    Device::Delete();
    Keyboard::Delete();
    Timer::Delete();
    Shader::Delete();
    Texture::Delete();
    Environment::Delete();
    SceneManager::Delete();
    Mouse::Delete();
    Font::Delete();
    UIRenderMaster::Delete();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}
