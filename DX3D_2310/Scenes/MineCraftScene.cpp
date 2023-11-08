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
}

void MinceCraftScene::GUIRender()
{
}

void MinceCraftScene::CollisionandUp()
{
	Vector3 playerpos = player->GetLocalPosition();

	//for (auto& def : itemlist) {
	//	Contact scala;
	//	// 모든 객체와 충돌판정처리 중력처리임.
	//	if (def->IsRayCollision(player->GetDownRay(), &scala) && scala.distance <= player->Radius() * 2.1) {

	//		gravity = false;
	//		player->IsJump() = false;
	//		Vector3 setpos = player->GetLocalPosition();

	//		//if(scala.distance < 1)
	//		//	player->SetLocalPosition(playerpos + Vector3(0, (9.8 + player->GetMoveSpeed()) * DELTA, 0));
	//	}

	//	if (def->IsSphereCollision(player))
	//	{
	//		// 사각형에서 플레이어 쪽으로 발산하는 Vector구하기
	//		Vector3 Localvector = player->GetHitpoint() - def->GetLocalPosition();
	//		Localvector.Normalized();

	//		// 원래 플레이어의 반지름
	//		float BaseobjectToPlayer = player->Radius();

	//		// 플레이어의 포지션을. 현재 플레이어 위치 + 접촉점에서 플레이어쪽으로 가는 Vector * 현재 겹쳐 들어간 만큼의 거리.
	//		// 즉, 접촉점에서 플레이어방향으로 가해진 힘만큼 다시 뒤로 되돌려줌.
	//		// 벽에 테니스공이 힘을 가했을 때, 반탄력으로 되돌아오는걸 계산해서 다시 되돌려주는 vector
	//		// 벽이 고정되어있을 경우, 가해진 힘은 그대로 역방향으로 되돌려준다.
	//		player->SetLocalPosition(player->GetLocalPosition() + Localvector * (player->Radius() - player->Getdistance()));
	//	}
	//}
}
