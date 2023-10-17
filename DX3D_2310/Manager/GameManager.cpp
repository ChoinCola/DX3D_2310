#include "Framework.h"
#include "GameManager.h"

#include "Scenes/TutorialScene.h"
#include "Scenes/GridScene.h"

GameManager::GameManager()
{
	Create();

	SceneManager::Get()->Create("Grid", new GridScene());
	//SceneManager::Get()->Create("Start", new TutorialScene());
	SceneManager::Get()->Create("Game", new GameScene());

	SceneManager::Get()->Add("Grid");
	SceneManager::Get()->Add("Game");
}

GameManager::~GameManager()
{
	Delete();
}

void GameManager::Update()
{
	KEY->Update();
	Timer::Get()->Update();

	SceneManager::Get()->Update();
}

void GameManager::Render()
{
	SceneManager::Get()->PreRender();

	Device::Get()->Clear();

	SceneManager::Get()->Render();
	SceneManager::Get()->PostRender();
	SceneManager::Get()->GUIRender();

	Device::Get()->Present();
}

void GameManager::Create()
{
	Device::Get();
	Keyboard::Get();
	Timer::Get();
	Environment::Get();
	SceneManager::Get();
}

void GameManager::Delete()
{
	Device::Delete();
	Keyboard::Delete();
	Timer::Delete();
	Shader::Delete();
	Environment::Delete();
	SceneManager::Delete();
}
