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

	// 받은 위치좌표를 스크린좌표로 변환해주는 함수.
	Vector3 WorldToScreen(Vector3 worldPos);

	bool ContainPoint(Vector3 point); // 특정 점이 절도체 안에 들어오는지 아닌지 확인함.
	bool ContainSphere(Vector3 Cneter, float radius);

private:
	void FreeMode();

	// 절도체의 육면을 알아내서. 평면방정식을 사용해 면 안에 있는지 알아낸다.
	void Frustum();

private:
	ViewBuffer* viewBuffer;
	Matrix view;
	Matrix projection;
	float moveSpeed = 50.0f;
	float rotSpeed = 10.0f;

	Vector3 prevMousePos;

	Vector3 planes[6]; // 여섯개의 면을 만듬.
	float a, b, c, d;
};