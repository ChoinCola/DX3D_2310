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
	MouseBag::Get()->Update();
}

void Steve::GUIRender()
{
	ui->GUIRender();

}

void Steve::PostRender()
{
	ui->PostRender();
	MouseBag::Get()->Render();
}

void Steve::Render()
{
}

void Steve::Move()
{
	// 이동벡터에서 y값은 의미없음으로 y값은 항상 0으로 초기화해준다.
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
	// esc가 눌렸을 경우에만 이동을 반환한다.
	if (KEY->Down(VK_ESCAPE))
		isFree = !isFree;
	// Free모드일경우에는 그냥 이동벡터 적용안함.
	if (isFree) return;

	// 현재 delta값에 마우스 pos값을 받아옴.
	Vector3 delta = mousePos - Vector3(CENTER_X, CENTER_Y);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);

	// 회전값 적용
	Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
	Rotate(Vector3::Left() * delta.y * rotSpeed * DELTA);

	//localPosition.y = BlockManager::Get()->GetHeight(localPosition) + Radius();

	// 카메라 업데이트
	CAM->SetLocalPosition(localPosition);
	CAM->SetLocalRotation(localRotation);
}

// 써있는거는 점프라고 써져있지만, 사실 점프와 높이값설정을 같이사용한다.
void Steve::Jump()
{
	// 스페이스바가 눌리면 윗벡터를 Jump_Power로 설정
	if (KEY->Down(VK_SPACE))
		velocity = JUMP_POWER;

	// 윗벡터에 중력값 만큼 시간마다 빼줌
	velocity -= GRAVITY * DELTA;

	// 현재 위치값에 위로향하는 힘을 곱해준다.
	Translate(Vector3::Up() * velocity * DELTA);

	// 현재 위치값에서 Block에 대한 높이값을 검사한다.
	float height = BlockManager::Get()->GetHeight(localPosition);

	// 현재 y높이가. 충돌높이보다 작을경우. 블록 안에 끼어있다는 의미 임으로. velocity를 0으로.
	// 현재 높이값을 블록의 높이값 + 구체의 반지름으로 설정한다.
	if (height > localPosition.y - Radius())
	{
		velocity = 0.0f;
		localPosition.y = height + Radius();
	}
}

void Steve::Control()
{
	// ImGui에 마우스가 겹쳐있는지 확인.
	if (ImGui::GetIO().WantCaptureMouse)
		return;

	if (UIManager::Get()->IsMouseOver())
		return;

	// 왼쪽 버튼을 누르면 블럭을 캐는걸로 정의
	if (KEY->Down(VK_LBUTTON))
		ui->Mining();

	// 오른쪽 버튼을 누르면 블럭을 쌓는걸로 정의.
	if (KEY->Down(VK_RBUTTON))
		ui->Build();
}