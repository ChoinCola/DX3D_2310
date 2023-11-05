#include "Framework.h"
#include "RPG.h"

RPG::RPG()
{
	terrain = new Terrain(L"Textures/HeightMaps/Miro.png",1.0f,true);
	terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
	terrain->GetMaterial()->SetSpecularMap(L"Textures/Landscape/fieldstone_SM.tga");
	terrain->GetMaterial()->SetNormalMap(L"Textures/Landscape/fieldstone_NM.tga");

	Environment::Get()->GetLightBuffer()[0].GetData()->lights[0].isActive = true;
	player = new Player(Vector3(5, 5, 5));
	player->SetTerrain(terrain);

	itemlist.emplace_back(new Chair());
	itemlist.emplace_back(new Table());

}

RPG::~RPG()
{
}

void RPG::Update()
{
	for (auto& def : itemlist)
		def->Update();

	player->Update();
	terrain->UpdateWorld();
}

void RPG::PreRender()
{
}

void RPG::Render()
{
	for (auto& def : itemlist)
		def->Render();
	terrain->Render();
}

void RPG::PostRender()
{
	player->UIRender();
}

void RPG::GUIRender()
{
	for (auto& def : itemlist)
		def->GUIRender();
	terrain->GUIRender();
}
