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

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (MATH->Random(0, 100) <= 10)
			{
				Box* def = new Box();
				def->SetLocalPosition(Vector3(i, 1, j));
				itemlist.emplace_back(def);
				def = new Box();
				def->SetLocalPosition(Vector3(i, 2, j));
				itemlist.emplace_back(def);
			}
			Box* def = new Box();
			def->SetLocalPosition(Vector3(i, 0, j ));
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
	//player->Render();
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
		// 모든 객체와 충돌판정처리 중력처리임.
		if (def->IsRayCollision(player->GetDownRay(), &scala) && scala.distance <= player->Radius() * 2) {

			gravity = false;
			player->IsJump() = false;
			Vector3 setpos = player->GetLocalPosition();

			//if(scala.distance < 1)
			//	player->SetLocalPosition(playerpos + Vector3(0, (9.8 + player->GetMoveSpeed()) * DELTA, 0));
		}

		if (def->IsSphereCollision(player))
		{
			// 사각형에서 플레이어 쪽으로 발산하는 Vector구하기
			Vector3 Localvector =  player->GetLocalPosition() - def->GetLocalPosition();
			Localvector.Normalized();

			// 원래 플레이어의 반지름
			float BaseobjectToPlayer = player->Radius();

			// 플레이어의 포지션을. 현재 플레이어 위치 + 접촉점에서 플레이어쪽으로 가는 Vector * 현재 겹쳐 들어간 만큼의 거리.
			// 즉, 접촉점에서 플레이어방향으로 가해진 힘만큼 다시 뒤로 되돌려줌.
			// 벽에 테니스공이 힘을 가했을 때, 반탄력으로 되돌아오는걸 계산해서 다시 되돌려주는 vector
			// 벽이 고정되어있을 경우, 가해진 힘은 그대로 역방향으로 되돌려준다.
			player->SetLocalPosition(player->GetLocalPosition() + Localvector * (player->Radius() - player->Getdistance()));
		}
	}

	if(gravity)
		player->SetLocalPosition(playerpos + Vector3(0, -9.8 * DELTA, 0));
}
