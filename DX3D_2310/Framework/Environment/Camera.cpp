#include "Framework.h"

Camera::Camera()
{
	tag = "Camera";
	viewBuffer = new ViewBuffer();
	viewBuffer->SetVS(1);
	viewBuffer->SetPS(1);

	Load();
}

Camera::~Camera()
{
	Save();
	delete viewBuffer;
}

void Camera::Update()
{
	projection = Environment::Get()->GetProjection();
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
	viewBuffer->Set(view, world);

	viewBuffer->SetVS(1);
	viewBuffer->SetPS(1);
}

Ray Camera::ScreenPointToRay(Vector3 screenPoint)
{
	Vector3 screenSize(WIN_WIDTH, WIN_HEIGHT, 1.0f);

	Float2 point;
	point.x = (screenPoint.x / screenSize.x) * 2.0f - 1.0f;
	point.y = (screenPoint.y / screenSize.y) * 2.0f - 1.0f;

	Float4x4 temp;
	XMStoreFloat4x4(&temp, projection);

	screenPoint.x = point.x / temp._11;
	screenPoint.y = point.y / temp._22;
	screenPoint.z = 1.0f;

	screenPoint = XMVector3TransformNormal(screenPoint, world);
	// 카메라 행렬을 다시 곱해줘서 카메라 기준 반직선 구함.


	return Ray(localPosition, screenPoint.GetNormalized());
}

void Camera::FreeMode()
{
	Vector3 delta = Mouse::Get()->GetMoveValue();

	if (KEY->Press(VK_RBUTTON))
	{
		if (KEY->Press('W')) Translate(GetForward() * moveSpeed * DELTA);
		if (KEY->Press('S')) Translate(GetBack() * moveSpeed * DELTA);
		if (KEY->Press('A')) Translate(GetLeft() * moveSpeed * DELTA);
		if (KEY->Press('D')) Translate(GetRight() * moveSpeed * DELTA);
		if (KEY->Press('Q')) Translate(GetUp() * moveSpeed * DELTA);
		if (KEY->Press('E')) Translate(GetDown() * moveSpeed * DELTA);


		Rotate(Vector3::Up() * delta.x * rotSpeed * DELTA);
		Rotate(Vector3::Left() * -delta.y * rotSpeed * DELTA);

		//알아야 할것. 법선, 빛의 방향, 법선의 각도
		//법선 정규화
	}

}
