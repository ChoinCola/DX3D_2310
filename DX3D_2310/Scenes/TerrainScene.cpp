#include "Framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
	terrain->SetTag("terrain");
	terrain->GetMaterial()->SetName("terrain");
	//sphere = new Sphere(5.0f);
	//sphere->GetMetrial()->SetShader(L"Light/NormalMapping.hlsl");
	//sphere->GetMetrial()->SetDiffuseMap	(L"Textures/Landscape/Fieldstone_DM.tga");
	//sphere->GetMetrial()->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
	//sphere->GetMetrial()->SetNormalMap	(L"Textures/Landscape/Fieldstone_NM.tga");
}

TerrainScene::~TerrainScene()
{
	delete terrain;
}

void TerrainScene::Update()
{
	terrain->UpdateWorld();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();

}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
	terrain->GUIRender();
}
