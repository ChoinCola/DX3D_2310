#include "Framework.h"

Steve::Steve()
{
	localPosition = { 5, 10, 5 };

	prevMousePos = mousePos;
	ClientToScreen(hWnd, &clientCenterPos);

	ui = new MineCraftUI();
}

Steve::~Steve()
{
}

void Steve::Update()
{
	ui->Update();

	Move();
	BlockManager::Get()->CollisionBlockToPlayer(*this);
	Jump();
	Control();

	UpdateWorld();
}

void Steve::GUIRender()
{
	ui->GUIRender();
}

void Steve::PostRender()
{
	ui->PostRender();
}

void Steve::Render()
{
}

void Steve::Move()
{
	// �̵����Ϳ��� y���� �ǹ̾������� y���� �׻� 0���� �ʱ�ȭ���ش�.
	if (KEY->Press('W'))
	{
		Vector3 forward = GetForward();
		forward.y = 0;
		Translate(forward.GetNormalized() * moveSpeed * DELTA);
	}
	else if (KEY->Press('S'))
	{
		Vector3 back = GetBack();
		back.y = 0;
		Translate(back.GetNormalized() * moveSpeed * DELTA);
	}

	if (KEY->Press('A'))
	{
		Vector3 left = GetLeft();
		left.y = 0;
		Translate(left.GetNormalized() * moveSpeed * DELTA);
	}
	else if (KEY->Press('D'))
	{
		Vector3 right = GetRight();
		right.y = 0;
		Translate(right.GetNormalized() * moveSpeed * DELTA);
	}
	// esc�� ������ ��쿡�� �̵��� ��ȯ�Ѵ�.
	if (KEY->Down(VK_ESCAPE))
		isFree = !isFree;
	// Free����ϰ�쿡�� �׳� �̵����� �������.
	if (isFree) return;

	// ���� delta���� ���콺 pos���� �޾ƿ�.
	Vector3 delta = mousePos - Vector3(CENTER_X, CENTER_Y);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);

	// ȸ���� ����
	Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
	Rotate(Vector3::Left() * delta.y * rotSpeed * DELTA);

	//localPosition.y = BlockManager::Get()->GetHeight(localPosition) + Radius();

	// ī�޶� ������Ʈ
	CAM->SetLocalPosition(localPosition);
	CAM->SetLocalRotation(localRotation);
}

// ���ִ°Ŵ� ������� ����������, ��� ������ ���̰������� ���̻���Ѵ�.
void Steve::Jump()
{
	// �����̽��ٰ� ������ �����͸� Jump_Power�� ����
	if (KEY->Down(VK_SPACE))
		velocity = JUMP_POWER;

	// �����Ϳ� �߷°� ��ŭ �ð����� ����
	velocity -= GRAVITY * DELTA;

	// ���� ��ġ���� �������ϴ� ���� �����ش�.
	Translate(Vector3::Up() * velocity * DELTA);

	// ���� ��ġ������ Block�� ���� ���̰��� �˻��Ѵ�.
	float height = BlockManager::Get()->GetHeight(localPosition);

	// ���� y���̰�. �浹���̺��� �������. ��� �ȿ� �����ִٴ� �ǹ� ������. velocity�� 0����.
	// ���� ���̰��� ����� ���̰� + ��ü�� ���������� �����Ѵ�.
	if (height > localPosition.y - Radius())
	{
		velocity = 0.0f;
		localPosition.y = height + Radius();
	}
}

void Steve::Control()
{
	// ���� ��ư�� ������ ���� ĳ�°ɷ� ����
	if (KEY->Down(VK_LBUTTON))
		ui->Mining();

	// ������ ��ư�� ������ ���� �״°ɷ� ����.
	if (KEY->Down(VK_RBUTTON))
		ui->Build();
}
