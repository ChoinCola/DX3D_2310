#include "Framework.h"

Transform::Transform()
{
	world = XMMatrixIdentity();
}

void Transform::GUIRender()
{
	if (ImGui::TreeNode((tag + "_Tarnsform").c_str()))
		// 묶어주는 기능
	{
		string temp = tag + "_Pos";
		ImGui::DragFloat3(temp.c_str(), (float*)&localPosition, 0.1f);

		temp = tag + "_Rot";
		Float3 rot;
		rot.x = XMConvertToDegrees(localRotation.x);
		rot.y = XMConvertToDegrees(localRotation.y);
		rot.z = XMConvertToDegrees(localRotation.z);

		ImGui::DragFloat3(temp.c_str(), (float*)&rot, 1.0f, -180, 180); // -180에서 180까지


		localRotation.x = XMConvertToRadians(rot.x);
		localRotation.y = XMConvertToRadians(rot.y);
		localRotation.z = XMConvertToRadians(rot.z);

		temp = tag + "_Scale";
		ImGui::DragFloat3(temp.c_str(), (float*)&localScale, 0.1f);

		ImGui::TreePop();
	}
}

void Transform::UpdateWorld()
{
	world = XMMatrixTransformation(pivot, XMQuaternionIdentity(),
		localScale, pivot,
		XMQuaternionRotationRollPitchYawFromVector(localRotation),
		localPosition);

	if (parent) world *= parent->world;

	// pivot의 역할
	/*
		pivot만큼 곱해서 이동시키고 마지막에 역행렬로 사용해서 다시 음으로 곱해버리면 피봇 전값이 나온다.
	*/
}

void Transform::Translate(Vector3 direction)
{
	localPosition += direction;
}

void Transform::Rotate(Vector3 direction)
{
	localRotation += direction;
}
void Transform::SetRotate(Vector3 direction)
{
	localRotation = direction;
}
