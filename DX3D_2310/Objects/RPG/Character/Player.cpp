#include "Framework.h"
#include "Player.h"

Player::Player(Vector3 pos)
{
	SetLocalPosition(pos);
	SetLocalScale(Vector3(0.3, 0.3, 0.3));
	itemBox = new ItemBox();
	moveSpeed = 10;
	rotSpeed = 10;
	lightnum = Environment::Get()->SetLight();
	DownRay.dir = Vector3(0, -1, 0);
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

	 //�����̽��ٸ� ������ ���� ���°� �ƴ� ��.
	if (KEY->Down(VK_SPACE) && !Isjump)
	{
		if (JumpHight < MaxJumpHight)
		{
			JumpHight += JumpSpeed * DELTA ;
			SetLocalPosition(GetLocalPosition() + Vector3(0, (JumpSpeed + 9.8) * DELTA, 0));
		}
		Isjump = true;
	}

	if (!Isjump) {
		JumpHight = 0;
	}


	DownRay.pos = GetLocalPosition() + Vector3(0, Radius(), 0);
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

		//// ���� �� ������ üũ
		//if (terrain->ChackOnGround(GetLocalPosition())) {
		//	// ������ġ�� �ʵ� ���� ����
		//	localPosition.y = terrain->GetOnGrondPosition(localPosition, Vector3(0, Radius(), 0)).y;

		//	// ������ ��ġ�� �߷°��ӵ��� �����Ͽ� ���� �� ���� ������ �������� ��.
		//	Vector3 pos = GetLocalPosition();
		//	Vector3 slide = terrain->GetOngravityAcceleration(pos, Vector3(0, Radius(), 0));
		//	SetLocalPosition(pos + slide * 9.8 * weight * DELTA);
		//}

		Vector3 delta = Mouse::Get()->GetMoveValue();

		Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
		Rotate(Vector3::Left() * -delta.y * rotSpeed * DELTA);

		Vector3 MoveCam = GetBack() * 5 + Vector3(0, 2, 0);

		CAM->SetLocalPosition(localPosition + MoveCam);
		CAM->SetLocalRotation(localRotation);
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
