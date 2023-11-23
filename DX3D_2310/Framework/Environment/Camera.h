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

	void TargetOptionSave(string file);
	void TargetOptionLoad(string file);

	void SetTarget(Transform* target) { this->target = target; }

private:
	void FreeMode();
	void FollowMode();

	// ����ü�� ������ �˾Ƴ���. ���������� ����� �� �ȿ� �ִ��� �˾Ƴ���.
	void Frustum();

private:
	ViewBuffer* viewBuffer;
	Matrix view;
	Matrix projection;
	float moveSpeed = 50.0f;
	float rotSpeed = 10.0f;

	Vector3 prevMousePos;
	
	// TargetOption
	Transform* target = nullptr;

	float distance = 20.0f;
	float height = 20.0f;
	float moveDamping = 5.0f; // ���󰡴� �ӵ�
	float rotDamping = 0.0f; // ȸ������ ���󰡴� �ӵ�
	float rotY = 0;			// ���ͺ�� �Ͽ��� ��, 45�������� �ٶ󺸰� ���� �����ֱ� ����,

	float destRot = 0.0f;
	Vector3 destPos;

	Vector3 focusOffset;	// ī�޶��� Ÿ�� ������

	// Ư�� Ÿ���� �ٶ󺸴µ� X�� �ޱ۷θ� �ȹٶ󺸰� �ϰ���� �� �ִ�.
	// �� ���� ī�޶� �̵��ϰ�
	bool isLookAtTargetX = true;
	bool isLookAtTargetY = true;

	// ȸ����� ����
	Matrix rotMatrix;

	char file[128];

	Vector3 planes[6]; // �������� ���� ����.
	float a, b, c, d;
};