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
	SAFE_DELETE(ui);
}

void Steve::Update()
{
	ui->Update();

	Move();
	BlockManager::Get()->CollisionBlockToPlayer(*this);
	Jump();
	Control();

	UpdateWorld();
	Observer::Get()->ExcuteParamEvent("SellerCollision", this);

}

void Steve::GUIRender()
{
	ui->GUIRender();

	ImGui::Text(to_string(GetLocalRotation().x).c_str());
	ImGui::DragFloat3("CamPos", &v[0], 1.0f, -100.0f, 100.0f);
	ImGui::DragFloat3("CamRot", &r[0], 1.0f, -100.0f, 100.0f);

	campos = Vector3(v[0], v[1], v[2]);
	camrot = Vector3(r[0], r[1], r[2]);
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
	// Free����ϰ�쿡�� �׳� �̵����� �������.
		// esc�� ������ ��쿡�� �̵��� ��ȯ�Ѵ�.
	if (KEY->Down(VK_ESCAPE))
		isFree = !isFree;
	CAM->SetParent(nullptr);
	if (isFree) return;

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
	else if (KEY->Press('A'))
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
	else
	{

	}
	// ���� delta���� ���콺 pos���� �޾ƿ�.
	Vector3 delta = mousePos - Vector3(CENTER_X, CENTER_Y);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);

	// ȸ���� ����
	Uprotation += Vector3::Up() * delta.x * rotSpeed * DELTA;



	Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);

	if (GetLocalRotation().x > 1)
		SetLocalRotation(Vector3(1, GetLocalRotation().y, GetLocalRotation().z));
	else if (GetLocalRotation().x < -0.1)
		SetLocalRotation(Vector3(-0.1, GetLocalRotation().y, GetLocalRotation().z));

	Rotate(Vector3::Left() * delta.y * rotSpeed * DELTA);

	//localPosition.y = BlockManager::Get()->GetHeight(localPosition) * 20;

	// ī�޶� ������Ʈ
	CAM->SetParent(this);
	CAM->SetLocalPosition(campos);
	CAM->SetLocalRotation(camrot);
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
	// ImGui�� ���콺�� �����ִ��� Ȯ��.
	if (ImGui::GetIO().WantCaptureMouse)
		return;

	if (UIManager::Get()->IsMouseOver())
		return;

	// ���� ��ư�� ������ ���� ĳ�°ɷ� ����
	if (KEY->Down(VK_LBUTTON))
		ui->Mining();

	// ������ ��ư�� ������ ���� �״°ɷ� ����.
	if (KEY->Down(VK_RBUTTON))
		ui->Build();
}