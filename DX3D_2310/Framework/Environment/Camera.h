#pragma once

class Camera : public Transform
{
public:
	Camera();
	~Camera();

	void Update();
	void GUIRender();

	void SetView();
	Ray ScreenPointToRay(Vector3 screenPoint);

	Matrix GetViewMatrix() { return view; }

	// ���� ��ġ��ǥ�� ��ũ����ǥ�� ��ȯ���ִ� �Լ�.
	Vector3 WorldToScreen(Vector3 worldPos);

	bool ContainPoint(Vector3 point); // Ư�� ���� ����ü �ȿ� �������� �ƴ��� Ȯ����.
	bool ContainSphere(Vector3 Cneter, float radius);

private:
	void FreeMode();

	// ����ü�� ������ �˾Ƴ���. ���������� ����� �� �ȿ� �ִ��� �˾Ƴ���.
	void Frustum();

private:
	ViewBuffer* viewBuffer;
	Matrix view;
	Matrix projection;
	float moveSpeed = 50.0f;
	float rotSpeed = 10.0f;

	Vector3 prevMousePos;

	Vector3 planes[6]; // �������� ���� ����.
	float a, b, c, d;
};