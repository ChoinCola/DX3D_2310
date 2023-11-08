#include "Framework.h"
#include "MineCraftScene.h"

MinceCraftScene::MinceCraftScene()
{
	BlockDataManager::Get()->LoadTable();
	BlockManager::Get()->CreateBlocks(10, 3, 10);
	Environment::Get()->GetLightBuffer()->GetData()->lights[0].isActive = true;

	player = new Steve();
}

MinceCraftScene::~MinceCraftScene()
{
	BlockManager::Delete();
	BlockDataManager::Delete();

	delete player;
}

void MinceCraftScene::Update()
{
	player->Update();
	BlockManager::Get()->Update();
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
	player->PostRender();
}

void MinceCraftScene::GUIRender()
{
	player->GUIRender();

}

void MinceCraftScene::CollisionandUp()
{
	Vector3 playerpos = player->GetLocalPosition();

	//for (auto& def : itemlist) {
	//	Contact scala;
	//	// ��� ��ü�� �浹����ó�� �߷�ó����.
	//	if (def->IsRayCollision(player->GetDownRay(), &scala) && scala.distance <= player->Radius() * 2.1) {

	//		gravity = false;
	//		player->IsJump() = false;
	//		Vector3 setpos = player->GetLocalPosition();

	//		//if(scala.distance < 1)
	//		//	player->SetLocalPosition(playerpos + Vector3(0, (9.8 + player->GetMoveSpeed()) * DELTA, 0));
	//	}

	//	if (def->IsSphereCollision(player))
	//	{
	//		// �簢������ �÷��̾� ������ �߻��ϴ� Vector���ϱ�
	//		Vector3 Localvector = player->GetHitpoint() - def->GetLocalPosition();
	//		Localvector.Normalized();

	//		// ���� �÷��̾��� ������
	//		float BaseobjectToPlayer = player->Radius();

	//		// �÷��̾��� ��������. ���� �÷��̾� ��ġ + ���������� �÷��̾������� ���� Vector * ���� ���� �� ��ŭ�� �Ÿ�.
	//		// ��, ���������� �÷��̾�������� ������ ����ŭ �ٽ� �ڷ� �ǵ�����.
	//		// ���� �״Ͻ����� ���� ������ ��, ��ź������ �ǵ��ƿ��°� ����ؼ� �ٽ� �ǵ����ִ� vector
	//		// ���� �����Ǿ����� ���, ������ ���� �״�� ���������� �ǵ����ش�.
	//		player->SetLocalPosition(player->GetLocalPosition() + Localvector * (player->Radius() - player->Getdistance()));
	//	}
	//}
}
