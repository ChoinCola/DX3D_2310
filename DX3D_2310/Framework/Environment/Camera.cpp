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
	Frustum();

	if (target)
		FollowMode();
	else
		FreeMode();

	FreeMode();
	Frustum();
	UpdateWorld();
}

void Camera::GUIRender()
{
	if (ImGui::TreeNode("CameraOption"))
	{
		ImGui::DragFloat("MoveSpeed", &moveSpeed);
		ImGui::DragFloat("RotSpeed", &rotSpeed);

		if (target)
		{
			ImGui::DragFloat("Distance", &distance, 0.1f);
			ImGui::DragFloat("Height", &height, 0.1f);
			ImGui::DragFloat3("ForcusOffset", (float*)&focusOffset, 0.1f);

			float degree = XMConvertToDegrees(rotY);
			ImGui::DragFloat("RotY", &degree, 0.1f);
			rotY = XMConvertToRadians(degree);

			ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f);
			ImGui::DragFloat("RotDamping", &rotDamping, 0.1f);

			ImGui::Checkbox("LookAtTargetX", &isLookAtTargetX);
			ImGui::Checkbox("LookAtTargetY", &isLookAtTargetY);

			ImGui::InputText("File", file, 128);

			if (ImGui::Button("Save"))
				TargetOptionSave(file);

			ImGui::SameLine();

			if (ImGui::Button("Load"))
				TargetOptionLoad(file);
		}

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

void Camera::TargetOptionSave(string file)
{
	string path = "TextData/Camera/" + file + ".cam";

	BinaryWriter* writer = new BinaryWriter(path);

	writer->Float(distance);
	writer->Float(height);
	writer->Float(moveDamping);
	writer->Float(rotDamping);
	writer->Float(rotY);

	writer->Vector(focusOffset);

	writer->Bool(isLookAtTargetX);
	writer->Bool(isLookAtTargetY);

	delete writer;
}

void Camera::TargetOptionLoad(string file)
{
	string path = "TextData/Camera/" + file + ".cam";

	BinaryReader* reader = new BinaryReader(path);

	distance = reader->Float();
	height = reader->Float();
	moveDamping = reader->Float();
	rotDamping = reader->Float();
	rotY = reader->Float();

	focusOffset = reader->Vector();

	isLookAtTargetX = reader->Bool();
	isLookAtTargetY = reader->Bool();

	delete reader;
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

void Camera::FollowMode()
{
	// 대상 회전을 부드럽게 따라가기 위해 보간 (Lerp)을 사용합니다.
	destRot = MATH->Lerp(destRot, target->GetLocalRotation().y, rotDamping * DELTA);

	// 회전 행렬을 계산하고 보간된 회전을 적용합니다.
	rotMatrix = XMMatrixRotationY(destRot + rotY);

	// 세계 공간에서의 전방 벡터를 계산합니다.
	Vector3 forward = XMVector3TransformNormal(Vector3::Forward(), rotMatrix);

	// 목표 위치를 지정된 거리와 높이로 대상 뒤에 설정합니다.
	destPos = target->GetGlobalPosition() - forward * distance;
	destPos.y += height;

	// 부드럽게 카메라를 대상 위치로 이동시킵니다.
	localPosition = MATH->Lerp(localPosition, destPos, moveDamping * DELTA);

	// 최종 대상 위치를 얻기 위해 포커스 오프셋을 적용합니다.
	Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
	Vector3 targetPos = target->GetGlobalPosition() + offset;

	// 카메라에서 대상까지의 방향을 계산하고 정규화합니다.
	Vector3 dir = (targetPos - localPosition).GetNormalized();

	// 수직 회전을 무시하고 XZ 평면으로 방향을 투영합니다.
	forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

	// 활성화된 경우, 카메라가 X축을 따라 대상을 보도록 로컬 회전을 업데이트합니다.
	if (isLookAtTargetX)
		localRotation.x = acos(Vector3::Dot(forward, dir));

	// 활성화된 경우, 카메라가 Y축을 따라 대상을 보도록 로컬 회전을 업데이트합니다.
	if (isLookAtTargetY)
		localRotation.y = atan2(dir.x, dir.z);
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
