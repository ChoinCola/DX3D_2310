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
		// ��� ��ü�� �浹����ó�� �߷�ó����.
		if (def->IsRayCollision(player->GetDownRay(), &scala) && scala.distance <= player->Radius() * 2) {

			gravity = false;
			player->IsJump() = false;
			//player->SetLocalPosition(playerpos);
			//if(scala.distance < 1)
			//	player->SetLocalPosition(playerpos + Vector3(0, (9.8 + player->GetMoveSpeed()) * DELTA, 0));
		}

		if (def->IsSphereCollision(player))
		{
			// �簢������ �÷��̾� ������ �߻��ϴ� Vector���ϱ�
			Vector3 Localvector = player->GetLocalPosition() - def->GetLocalPosition();
			Localvector.Normalized();

			// �簢���� �÷��̾ ������ �� ���ϱ�
			// �簢���� ��ġ���� �÷��̾������� ���ϴ� vector�� �簢���� �غ� ���� ���̸� �����ָ��.
			Vector3 Rectcontactpoint = def->GetLocalPosition();
			Rectcontactpoint *= Vector3::Dot(Localvector, )

			// ���������� �÷��̾� ������ ���ϴ� Vector���ϱ�
			Vector3 BackMoveVector = player->GetLocalPosition() - Rectcontactpoint;
			BackMoveVector.Normalized();

			// �÷��̾�� ������������ �Ÿ� ���ϱ�.(�����־��ٸ�, ����� �� �پ����̴�.)
			float NowobjectToPlayer = (Rectcontactpoint - player->GetLocalPosition()).Length();

			// ���� �÷��̾��� ������
			float BaseobjectToPlayer = player->Radius();

			// �÷��̾��� ��������. ���� �÷��̾� ��ġ + ���������� �÷��̾������� ���� Vector * ���� ���� �� ��ŭ�� �Ÿ�.
			// ��, ���������� �÷��̾�������� ������ ����ŭ �ٽ� �ڷ� �ǵ�����.
			// ���� �״Ͻ����� ���� ������ ��, ��ź������ �ǵ��ƿ��°� ����ؼ� �ٽ� �ǵ����ִ� vector
			// ���� �����Ǿ����� ���, ������ ���� �״�� ���������� �ǵ����ش�.
			player->SetLocalPosition(player->GetLocalPosition() + BackMoveVector * abs(BaseobjectToPlayer - NowobjectToPlayer));
		}
	}

	if(gravity)
		player->SetLocalPosition(playerpos + Vector3(0, -9.8 * DELTA, 0));
}
