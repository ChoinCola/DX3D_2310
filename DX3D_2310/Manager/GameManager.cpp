#include "Framework.h"
#include "GameManager.h"

#include "Scenes/TutorialScene.h"
#include "Scenes/GridScene.h"

GameManager::GameManager()
{
	Create();

	SceneManager::Get()->Create("Grid", new GridScene());
	SceneManager::Get()->Create("Start", new TutorialScene());
	SceneManager::Get()->Create("Terrain", new TerrainScene());

	//SceneManager::Get()->Create("Game", new GameScene());

	SceneManager::Get()->Add("Grid");
	//SceneManager::Get()->Add("Game");
	//SceneManager::Get()->Add("Start");
	SceneManager::Get()->Add("Terrain");

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
	SceneManager::Get()->PostRender();

	// �׻� �⺻������ �ٷ���־���ϴ� Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
	ImGui::Text(fps.c_str());

	
	Environment::Get()->GUIRender();
	//if (Mouse::Get()->GetMoveValue().x != 0 && Mouse::Get()->GetMoveValue().y != 0)
	//	Movevalue = { Mouse::Get()->GetMoveValue().x, Mouse::Get()->GetMoveValue().y };
	////	���콺 ��ġ��ǥ�� ���� xy����� ��������� �̵��߾����� ǥ��
	//string CusorMove = "PointPos : x." + to_string(Movevalue.x) + " y." + to_string(Movevalue.y);
	//ImGui::Text(CusorMove.c_str());

	//// ���� ī�޶��� �̵��ӵ��� �����ͼ� ǥ��, �����̴��� ����
	//float& speed = Environment::Get()->GetCamSpeed();
	//string camspeed = "CamSpeed" + to_string(speed);
	//ImGui::Text(camspeed.c_str());
	//ImGui::SliderFloat("Slice", (float*)&speed, 0, 100);
	//SceneManager::Get()->GUIRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Device::Get()->Present();
}

void GameManager::Create()
{
	Device::Get();
	Mouse::Get();
	Keyboard::Get();
	Timer::Get();
	Environment::Get();
	SceneManager::Get();

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

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}
