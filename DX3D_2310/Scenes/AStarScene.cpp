#include "Framework.h"

AStarScene::AStarScene()
{
	terrain = new Terrain();
	astar = new AStar();
	astar->SetNode(terrain);

	lion = new Lion();
	lion->SetTerrain(terrain);
	lion->SetAStar(astar);
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete lion;
	delete astar;
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
	astar->Render();
	lion->Render();

}

void AStarScene::PostRender()
{
}

void AStarScene::GUIRender()
{
	terrain->GUIRender();
	lion->GUIRender();
}
