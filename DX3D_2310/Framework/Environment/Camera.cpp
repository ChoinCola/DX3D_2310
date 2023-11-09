#include "Framework.h"

Camera::Camera()
{
	tag = "Camera";
	viewBuffer = new ViewBuffer();
	viewBuffer->SetVS(1);
	viewBuffer->SetPS(1);
	prevMousePos = mousePos;
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
	Frustum();
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

Vector3 Camera::WorldToScreen(Vector3 worldPos)
{
	Vector3 screenPos;

	screenPos = XMVector3TransformCoord(worldPos, view); // 월드위치를 view와 곱함
	screenPos = XMVector3TransformCoord(screenPos, projection);
	// NDC : -1 ~ 1 사이의 좌표가 나타남.

	screenPos = (screenPos + Vector3(1, 1, 1)) * 0.5f; //   0~1

	screenPos.x *= WIN_WIDTH;
	screenPos.y *= WIN_HEIGHT;
	// 받은 위치를 스크린좌표로 변환해준다.

	return screenPos;
}

bool Camera::ContainPoint(Vector3 point)
{
	FOR(6)
	{
		Vector3 dot = XMPlaneDot(planes[i], point);

		if (dot.x < 0.0f)
			return false;
	}
	return true;
}

bool Camera::ContainSphere(Vector3 center, float radius)
{
	Vector3 edge;
	Vector3 dot;

	FOR(6)
	{
		//1
		edge.x = center.x - radius;
		edge.y = center.y - radius;
		edge.z = center.z - radius;
		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		//2
		edge.x = center.x + radius;
		edge.y = center.y - radius;
		edge.z = center.z - radius;
		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		//3
		edge.x = center.x - radius;
		edge.y = center.y + radius;
		edge.z = center.z - radius;
		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		//4
		edge.x = center.x - radius;
		edge.y = center.y - radius;
		edge.z = center.z + radius;
		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		//5
		edge.x = center.x + radius;
		edge.y = center.y + radius;
		edge.z = center.z - radius;
		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		//6
		edge.x = center.x + radius;
		edge.y = center.y - radius;
		edge.z = center.z + radius;
		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		//7
		edge.x = center.x - radius;
		edge.y = center.y + radius;
		edge.z = center.z + radius;
		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		//8
		edge.x = center.x + radius;
		edge.y = center.y + radius;
		edge.z = center.z + radius;
		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		return false;
	}

	return true;
}

void Camera::FreeMode()
{
	//Vector3 delta = Mouse::Get()->GetMoveValue();
	Vector3 delta = mousePos - prevMousePos;
	prevMousePos = mousePos;

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

void Camera::Frustum()
{
	Float4x4 VP;
	XMStoreFloat4x4(&VP, view * projection); // 월드상의 절도체 정보

	// Left
	a = VP._14 + VP._11;
	b = VP._24 + VP._21;
	c = VP._34 + VP._31;
	d = VP._44 + VP._41;

	planes[0] = XMVectorSet(a, b, c, d);

	// Right
	a = VP._14 - VP._11;
	b = VP._24 - VP._21;
	c = VP._34 - VP._31;
	d = VP._44 - VP._41;

	planes[1] = XMVectorSet(a, b, c, d);

	// Bottom
	a = VP._14 + VP._12;
	b = VP._24 + VP._22;
	c = VP._34 + VP._32;
	d = VP._44 + VP._42;

	planes[2] = XMVectorSet(a, b, c, d);

	// TOP
	a = VP._14 - VP._12;
	b = VP._24 - VP._22;
	c = VP._34 - VP._32;
	d = VP._44 - VP._42;

	planes[3] = XMVectorSet(a, b, c, d);

	// Near
	a = VP._14 + VP._13;
	b = VP._24 + VP._23;
	c = VP._34 + VP._33;
	d = VP._44 + VP._43;

	planes[4] = XMVectorSet(a, b, c, d);

	// Far
	a = VP._14 - VP._13;
	b = VP._24 - VP._23;
	c = VP._34 - VP._33;
	d = VP._44 - VP._43;

	planes[5] = XMVectorSet(a, b, c, d);

	FOR(6)
		planes[i] = XMPlaneNormalize(planes[i]);
}
