#include "Framework.h"
#include "Map.h"

Map::Map(Collider* player, Float2 size) : player(player)
{
	map.reserve(9);
	MakeTerrain();
}

Map::~Map()
{
	for (auto& def : map)
		delete def;
}

void Map::Update()
{
	for (auto& def : map) {
		MoveTerrain();
		def->UpdateWorld();
	}
}

void Map::Render()
{
	for (auto& def : map) {
		def->Render();
		pair<float, float> input(def->GetLocalPosition().x, def->GetLocalPosition().z);
	}
}

void Map::GUIRender()
{
}

void Map::SetScail()
{
}

void Map::MakeTerrain()
{
	map.reserve(9);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			Terrain* ter = new Terrain();
			ter->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Floor.png");
			ter->GetMaterial()->SetNormalMap(L"Textures/Landscape/Floor_normal.png");

			defaltsize.x = ter->GetWidth()-1;
			defaltsize.y = ter->GetHeight()-1;
			ter->SetLocalPosition(Vector3((i - 1.5) * defaltsize.x, 0, (j - 1.5) * defaltsize.y));
			ter->UpdateWorld();
			map.push_back(ter);
		}
}

void Map::MoveTerrain()
{
	for (auto& def : map)
	{
		Vector3 pos(def->GetLocalPosition().x + defaltsize.x * 0.5 ,0 ,def->GetLocalPosition().z + defaltsize.y * 0.5);

		if (pos == player->GetLocalPosition()) continue;
		Vector3 nowtoplayer = pos - player->GetLocalPosition();

		float xp = Vector3::Dot(nowtoplayer, Vector3(+1, 0, 0));
		if (xp < 0) xp = 0;
		float xb = Vector3::Dot(nowtoplayer, Vector3(-1, 0, 0));
		if (xb < 0) xb = 0;
		float zp = Vector3::Dot(nowtoplayer, Vector3(0, 0, +1));
		if (zp < 0) zp = 0;
		float zb = Vector3::Dot(nowtoplayer, Vector3(0, 0, -1));
		if (zb < 0) zb = 0;

		Float2 distance = defaltsize;
		distance.x = distance.x * 1.5;
		distance.y = distance.y * 1.5;

		Vector3 movepos = {};
		
		// 우측으로 플레이어가 이동했을 경우,
		if (xp > distance.x)
			movepos.x = -defaltsize.x*3;

		// 좌측으로 플레이어가 이동했을 경우,
		if (xb > distance.x)
			movepos.x = defaltsize.x*3;

		// 위쪽으로 플레이어가 이동했을 경우,
		if (zp > distance.y)
			movepos.z = -defaltsize.y*3;

		// 아래쪽으로 플레이어가 이동했을 경우,
		if (zb > distance.y)
			movepos.z = defaltsize.y*3;

		if(movepos != Vector3(0, 0, 0))
			def->SetLocalPosition(def->GetLocalPosition() + movepos);
	}
}
