#include "Framework.h"

Transform::Transform()
{
	world = XMMatrixIdentity();
}

void Transform::GUIRender()
{
	if (ImGui::TreeNode((tag + "_Tarnsform").c_str()))
		// �����ִ� ���
	{
		string temp = tag + "_Pos";
		ImGui::DragFloat3(temp.c_str(), (float*)&localPosition, 0.1f);

		temp = tag + "_Rot";
		Float3 rot;
		rot.x = XMConvertToDegrees(localRotation.x);
		rot.y = XMConvertToDegrees(localRotation.y);
		rot.z = XMConvertToDegrees(localRotation.z);

		ImGui::DragFloat3(temp.c_str(), (float*)&rot, 1.0f, -180, 180); // -180���� 180����


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

	// pivot�� ����
	/*
		pivot��ŭ ���ؼ� �̵���Ű�� �������� ����ķ� ����ؼ� �ٽ� ������ ���ع����� �Ǻ� ������ ���´�.
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
