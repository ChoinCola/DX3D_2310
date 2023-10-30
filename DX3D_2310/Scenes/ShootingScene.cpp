#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{

	terrain = new Terrain();
	player = new ShootingPlayer(Vector3(10, 0, 10));
	player->SetTerrain(terrain);
}

ShootingScene::~ShootingScene()
{
	delete terrain;
	delete player;
}

void ShootingScene::Update()
{
	player ->Update();
}

void ShootingScene::PreRender()
{

}

void ShootingScene::Render()
{
	player->Render();
	terrain->Render();
}

void ShootingScene::PostRender()
{
	player->PostRender();
}

void ShootingScene::GUIRender()
{
}
