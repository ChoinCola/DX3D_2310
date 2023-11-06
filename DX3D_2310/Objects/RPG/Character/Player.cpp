#include "Framework.h"
#include "Player.h"

Player::Player(Vector3 pos)
{
	SetLocalPosition(pos);
	itemBox = new ItemBox();
	moveSpeed = 10;
	rotSpeed = 10;
	lightnum = Environment::Get()->SetLight();
	DownRay.dir = GetDown();
	DownRay.dir.Normalized();

	Load();
}

Player::~Player()
{
	Save();
}

void Player::Update()
{
	if (KEY->Down(VK_F3))
		isInplay = !isInplay;

	if (KEY->Down('I') || (itemBox->CloaseCollisionCheck() && !itemBox->GetIsCloase())) {
		IsOpenItemBox = !IsOpenItemBox;
		itemBox->GetIsCloase() = !IsOpenItemBox;
	}

	// 스페이스바를 눌렀고 점프 상태가 아닐 때.
	//if (KEY->Down(VK_SPACE) && !Isjump)
	//{
	//	if (JumpHight < MaxJumpHight)
	//	{
	//		JumpHight += JumpSpeed * DELTA -9.8;
	//		SetLocalPosition(GetLocalPosition() + Vector3(0, JumpSpeed * DELTA, 0));
	//	}
	//}

	if (!Isjump) {
		JumpHight = 0;
	}

	{
		DownRay.pos = GetLocalPosition();
	}

	itemBox->Update();
	UpdateWorld();
	Move();
}

void Player::Render()
{
	__super::Render();
}

void Player::GUIRender()
{
}

void Player::UIRender()
{
	if(IsOpenItemBox)
		itemBox->RenderUI();
}

void Player::SetIsUI()
{
}

void Player::SetIsItemBox()
{

}

void Player::SetTerrain(Terrain* terrain)
{
	this->terrain = terrain;
}

void Player::GetItem(Item& item)
{

}

void Player::OutItem(Item& item)
{

}

void Player::Move()
{
	Vector3 MoveVector = GetLocalPosition();

	if (isInplay) {
		if (KEY->Press('W')) SetLocalPosition(MoveVector + GetForward()	* moveSpeed * DELTA);
		if (KEY->Press('S')) SetLocalPosition(MoveVector + GetBack()	* moveSpeed * DELTA);
		if (KEY->Press('A')) SetLocalPosition(MoveVector + GetLeft()	* moveSpeed * DELTA);
		if (KEY->Press('D')) SetLocalPosition(MoveVector + GetRight()	* moveSpeed * DELTA);

		//// 현재 땅 위인지 체크
		//if (terrain->ChackOnGround(GetLocalPosition())) {
		//	// 현재위치를 필드 위로 보정
		//	localPosition.y = terrain->GetOnGrondPosition(localPosition, Vector3(0, Radius(), 0)).y;

		//	// 보정된 위치에 중력가속도를 보정하여 기울기 에 따라 밑으로 내려가게 함.
		//	Vector3 pos = GetLocalPosition();
		//	Vector3 slide = terrain->GetOngravityAcceleration(pos, Vector3(0, Radius(), 0));
		//	SetLocalPosition(pos + slide * 9.8 * weight * DELTA);
		//}

		Vector3 delta = Mouse::Get()->GetMoveValue();

		Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
		Rotate(Vector3::Left() * -delta.y * rotSpeed * DELTA);

		//CAM->SetLocalPosition(localPosition);
		//CAM->SetLocalRotation(localRotation);
	}

}

void Player::Load()
{

}

void Player::Save()
{

}

void Player::OpenItemBox()
{

}
