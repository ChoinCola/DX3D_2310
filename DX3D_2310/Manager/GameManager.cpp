#include "Framework.h"
#include "GameManager.h"


GameManager::GameManager()
{
    Create();


    SceneManager::Get()->Create("Grid", new GridScene());
    //SceneManager::Get()->Create("Collision", new CollisionScene());
    //SceneManager::Get()->Create("Exporter", new ModelExportScene());
    //SceneManager::Get()->Create("Start", new ModelRenderScene());
    SceneManager::Get()->Create("RPG", new RPG());
    //SceneManager::Get()->Create("MineCraftScene", new MinceCraftScene());

    SceneManager::Get()->Add("Grid");
    //SceneManager::Get()->Add("MineCraftScene");
    //SceneManager::Get()->Add("Collision");
    SceneManager::Get()->Add("RPG");
    //SceneManager::Get()->Add("Exporter");
    //SceneManager::Get()->Add("Start");
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
}

void GameManager::Render()
{
    SceneManager::Get()->PreRender();

    Device::Get()->Clear();

    Environment::Get()->Set();
    SceneManager::Get()->Render();

    Environment::Get()->SetPost();
    SceneManager::Get()->PostRender();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    ImGui::Text(fps.c_str());

    Environment::Get()->GUIRender();
    SceneManager::Get()->GUIRender();

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
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}
