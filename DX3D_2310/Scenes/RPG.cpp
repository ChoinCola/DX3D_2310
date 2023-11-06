#include "Framework.h"
#include "RPG.h"

RPG::RPG()
{
	terrain = new Terrain(L"Textures/HeightMaps/Miro.png",1.0f,true);
	terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
	terrain->GetMaterial()->SetSpecularMap(L"Textures/Landscape/fieldstone_SM.tga");
	terrain->GetMaterial()->SetNormalMap(L"Textures/Landscape/fieldstone_NM.tga");

	Environment::Get()->GetLightBuffer()[0].GetData()->lights[0].isActive = true;
	player = new Player(Vector3(2, 5, 2));
	player->SetTerrain(terrain);

	itemlist.emplace_back(new Chair());
	itemlist.emplace_back(new Table());

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			Box* def = new Box();
			def->SetLocalPosition(Vector3(i * 2, 0, j * 2));
			itemlist.emplace_back(def);
		}
	}

	Environment::Get()->GetLightBuffer()->GetData()->lights[0].isActive = true;
}

RPG::~RPG()
{
}

void RPG::Update()
{
	for (auto& def : itemlist)
		def->Update();

	player->Update();
	CollisionandUp();
	//terrain->UpdateWorld();
}

void RPG::PreRender()
{
}

void RPG::Render()
{
	for (auto& def : itemlist)
		def->Render();
	player->Render();
	//terrain->Render();
}

void RPG::PostRender()
{
	player->UIRender();
}

void RPG::GUIRender()
{
	for (auto& def : itemlist)
		def->GUIRender();
	//terrain->GUIRender();
}

void RPG::CollisionandUp()
{
	Vector3 playerpos = player->GetLocalPosition();
	bool gravity = true;

	for (auto& def : itemlist) {
		Contact scala;
		Ray playerRay = player->GetDownRay();
		// 葛电 按眉客 面倒魄沥贸府 吝仿贸府烙.
		if (def->IsRayCollision(playerRay, &scala) && playerpos.y - def->GetLocalPosition().y <= 5) {
			player->SetLocalPosition(scala.hitPoint + Vector3(0, 5, 0));
			gravity = false;
		}
	}

	if(gravity)
		player->SetLocalPosition(playerpos + Vector3(0, -9.8 * DELTA, 0));
}
