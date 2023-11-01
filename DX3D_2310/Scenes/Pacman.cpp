#include "Framework.h"
#include "Pacman.h"

Pacman::Pacman()
{
	terrain = new Terrain(L"Textures/HeightMaps/Miro.png", 2, true);
	terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Floor2.png");
	//terrain->GetMaterial()->SetNormalMap(L"Textures/Landscape/Floor_normal.png");

	ceiling = new Terrain(L"Textures/Colors/Black.png", 2, true, true);
	ceiling->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/ceiling.png");
	ceiling->GetMaterial()->SetNormalMap(L"Textures/Landscape/Floor_normal.png");

	ceiling->SetLocalScale(terrain->GetLocalScale());
	ceiling->SetLocalPosition(Vector3(0, 5, 0));
	ceiling->GetMaterial()->SetSpecularMap(L"Textures/Colors/White.png");
	//terrain->GetMaterial()->SetNormalMap(L"Textures/Landscape/Floor_normal.png");

	Settile = new SamplerState();

	player = new ShootingPlayer(Vector3(10, 0, 10));
	player->SetTerrain(terrain);
}

Pacman::~Pacman()
{
	SAFE_DELETE(terrain);
	SAFE_DELETE(player);
}

void Pacman::Update()
{
	player->Update();
	ceiling->UpdateWorld();
}

void Pacman::PreRender()
{
}

void Pacman::Render()
{
	//player->Render();
	Render_terrain();

}

void Pacman::PostRender()
{
	player->PostRender();
}

void Pacman::GUIRender()
{
	ceiling->GUIRender();
}

void Pacman::Render_terrain()
{
	terrain->Render();
	ceiling->Render();
}
