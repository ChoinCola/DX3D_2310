#include "Framework.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
	terrain->SetTag("terrain");
	terrain->GetMetrial()->SetName("terrain");
	//sphere = new Sphere(5.0f);
	robot = new Robot({0, 0, 0}, 1);
	//sphere->GetMetrial()->SetShader(L"Light/NormalMapping.hlsl");
	//sphere->GetMetrial()->SetDiffuseMap	(L"Textures/Landscape/Fieldstone_DM.tga");
	//sphere->GetMetrial()->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
	//sphere->GetMetrial()->SetNormalMap	(L"Textures/Landscape/Fieldstone_NM.tga");
}

TerrainScene::~TerrainScene()
{
	delete terrain;
	//delete sphere;
	delete robot;
}

void TerrainScene::Update()
{
	robot->SetLocalPosition(terrain->GetOnGrondPosition(robot->GetLocalPosition()));
	terrain->UpdateWorld();
	//sphere->UpdateWorld();
	robot->Update();
	robot->UpdateWorld();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();
	//sphere->Render();
	robot->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
	terrain->GUIRender();
	robot->GUIRender();
	//sphere->GUIRender();
	//sphere->GetMetrial()->GUIRneder();
}
