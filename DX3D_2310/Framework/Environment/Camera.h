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
private:
	void FreeMode();

private:
	ViewBuffer* viewBuffer;
	Matrix view;
	Matrix projection;
	float moveSpeed = 50.0f;
	float rotSpeed = 10.0f;
	Vector3 prevMousePos;
};