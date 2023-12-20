#include "Framework.h"

AStarScene::AStarScene()
{
	terrain = new Terrain();
	astar = new AStar();
	astar->SetNode(terrain);

	lion = new Lion();
	lion->SetTerrain(terrain);
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete lion;
}

void AStarScene::Update()
{
	lion->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	terrain->Render();
	lion->Render();
	astar->Render();
}

void AStarScene::PostRender()
{
}

void AStarScene::GUIRender()
{
	terrain->GUIRender();
	lion->GUIRender();
}
