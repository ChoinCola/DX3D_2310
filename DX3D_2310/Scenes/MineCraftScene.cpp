#include "Framework.h"
#include "MineCraftScene.h"

MinceCraftScene::MinceCraftScene()
{
	BlockManager::Get()->CreateBlocks(10, 3, 10);
	Environment::Get()->GetLightBuffer()->GetData()->lights[0].isActive = true;
	player = new Player(Vector3(2, 5, 2));

}

MinceCraftScene::~MinceCraftScene()
{
	BlockManager::Delete();
}

void MinceCraftScene::Update()
{
	player->Update();
	CollisionandUp();
	BlockManager::Get()->Update();
	BlockManager::Get()->InsertBlocks(Environment::Get()->GetMainCamera()->ScreenPointToRay(Mouse::Get()->GetPosition()));
	BlockManager::Get()->DeleteBlocks(Environment::Get()->GetMainCamera()->ScreenPointToRay(Mouse::Get()->GetPosition()));
}

void MinceCraftScene::PreRender()
{
}

void MinceCraftScene::Render()
{
	BlockManager::Get()->Redner();
}

void MinceCraftScene::PostRender()
{
}

void MinceCraftScene::GUIRender()
{
}

void MinceCraftScene::CollisionandUp()
{
	Vector3 playerpos = player->GetLocalPosition();
	bool gravity = true;

	list<Block*> itemlist = BlockManager::Get()->GetBlocks();

	for (auto& def : itemlist) {
		Contact scala;
		// ��� ��ü�� �浹����ó�� �߷�ó����.
		if (def->IsRayCollision(player->GetDownRay(), &scala) && scala.distance <= player->Radius() * 2) {

			gravity = false;
			player->IsJump() = false;
			Vector3 setpos = player->GetLocalPosition();

			//if(scala.distance < 1)
			//	player->SetLocalPosition(playerpos + Vector3(0, (9.8 + player->GetMoveSpeed()) * DELTA, 0));
		}

		if (def->IsSphereCollision(player))
		{
			// �簢������ �÷��̾� ������ �߻��ϴ� Vector���ϱ�
			Vector3 Localvector = player->GetHitpoint() - def->GetLocalPosition();
			Localvector.Normalized();

			// ���� �÷��̾��� ������
			float BaseobjectToPlayer = player->Radius();

			// �÷��̾��� ��������. ���� �÷��̾� ��ġ + ���������� �÷��̾������� ���� Vector * ���� ���� �� ��ŭ�� �Ÿ�.
			// ��, ���������� �÷��̾�������� ������ ����ŭ �ٽ� �ڷ� �ǵ�����.
			// ���� �״Ͻ����� ���� ������ ��, ��ź������ �ǵ��ƿ��°� ����ؼ� �ٽ� �ǵ����ִ� vector
			// ���� �����Ǿ����� ���, ������ ���� �״�� ���������� �ǵ����ش�.
			player->SetLocalPosition(player->GetLocalPosition() + Localvector * (player->Radius() - player->Getdistance()));
		}
	}

	if (gravity)
		player->SetLocalPosition(playerpos + Vector3(0, -9.8 * DELTA, 0));
}
