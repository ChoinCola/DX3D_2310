#include "Framework.h"

Camera::Camera()
{

	viewBuffer = new MatrixBuffer();

}

Camera::~Camera()
{
	delete viewBuffer;
}

void Camera::Update()
{
	FreeMode();
	UpdateWorld();
}

void Camera::GUIRender()
{
	if (ImGui::TreeNode("CameraOption"))
	{
		ImGui::DragFloat("MoveSpeed", &moveSpeed);
		ImGui::DragFloat("MoveSpeed", &rotSpeed);

		ImGui::TreePop();
	}
}

void Camera::SetView()
{
	view = XMMatrixInverse(nullptr, world);

	viewBuffer->Set(view);
	viewBuffer->SetVS(1);
}

void Camera::FreeMode()
{
	Vector3 delta = Mouse::Get()->GetMoveValue();

	if (KEY->Press(VK_RBUTTON))
	{
		if (KEY->Press('W'))
			Translate(GetForward() * moveSpeed * DELTA);
		if (KEY->Press('S'))
			Translate(GetBack() * moveSpeed * DELTA);
		if (KEY->Press('A'))
			Translate(GetLeft() * moveSpeed * DELTA);
		if (KEY->Press('D'))
			Translate(GetRight() * moveSpeed * DELTA);
		if (KEY->Press('Q'))
			Translate(GetUp() * moveSpeed * DELTA);
		if (KEY->Press('E'))
			Translate(GetDown() * moveSpeed * DELTA);


		Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
		Rotate(Vector3::Left() * -delta.y * rotSpeed * DELTA);

		//알아야 할것. 법선, 빛의 방향, 법선의 각도
		//법선 정규화
	}

}
