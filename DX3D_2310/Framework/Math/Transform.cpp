#include "Framework.h"

Transform::Transform()
{
	world = XMMatrixIdentity();
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
