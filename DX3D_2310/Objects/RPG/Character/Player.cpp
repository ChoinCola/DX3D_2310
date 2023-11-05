#include "Framework.h"
#include "Player.h"

Player::Player(Vector3 pos)
{
	SetLocalPosition(pos);
	itemBox = new ItemBox();
	moveSpeed = 10;
	rotSpeed = 10;
	lightnum = Environment::Get()->SetLight();
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


	itemBox->Update();
	UpdateWorld();
	Move();
}

void Player::Render()
{
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

		// ���� �� ������ üũ
		if (terrain->ChackOnGround(GetLocalPosition())) {
			// ������ġ�� �ʵ� ���� ����
			localPosition.y = terrain->GetOnGrondPosition(localPosition, Vector3(0, Radius(), 0)).y;

			// ������ ��ġ�� �߷°��ӵ��� �����Ͽ� ���� �� ���� ������ �������� ��.
			Vector3 pos = GetLocalPosition();
			Vector3 slide = terrain->GetOngravityAcceleration(pos, Vector3(0, Radius(), 0));
			SetLocalPosition(pos + slide * 9.8 * weight * DELTA);
		}

		Vector3 delta = Mouse::Get()->GetMoveValue();

		Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
		Rotate(Vector3::Left() * -delta.y * rotSpeed * DELTA);

		CAM->SetLocalPosition(localPosition);
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
