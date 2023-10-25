#include "Framework.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
	sphere = new Sphere(5.0f);
	
	sphere->GetMetrial()->SetShader(L"Light/NormalMapping.hlsl");
	sphere->GetMetrial()->SetDiffuseMap	(L"Textures/Landscape/Fieldstone_DM.tga");
	sphere->GetMetrial()->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
	sphere->GetMetrial()->SetNormalMap	(L"Textures/Landscape/Fieldstone_NM.tga");
}

TerrainScene::~TerrainScene()
{
	delete terrain;
	delete sphere;
}

void TerrainScene::Update()
{
	terrain->UpdateWorld();
	sphere->UpdateWorld();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();
	sphere->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
	terrain->GUIRender();
	sphere->GUIRender();
	sphere->GetMetrial()->GUIRneder();
}
