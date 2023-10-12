#include "Framework.h"
#include "GameManager.h"

#include "Scenes/TutorialScene.h"

GameManager::GameManager()
{
	Create();

	scene = new TutorialScene();
}

GameManager::~GameManager()
{
	delete scene;

	Delete();
}

void GameManager::Update()
{
	KEY->Update();
	Timer::Get()->Update();

	scene->Update();
}

void GameManager::Render()
{
	scene->PreRneder();

	Device::Get()->Clear();

	scene->Render();
	scene->PostRender();
	scene->GUIRender();

	Device::Get()->Present();
}

void GameManager::Create()
{
	Device::Get();
	Keyboard::Get();
	Timer::Get();
}

void GameManager::Delete()
{
	Device::Delete();
}
