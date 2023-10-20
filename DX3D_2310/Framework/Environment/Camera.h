#pragma once

class Camera : public Transform
{
public:
	Camera();
	~Camera();

	void Update();
	void GUIRender();

	void SetView();
private:
	void FreeMode();

private:
	MatrixBuffer* viewBuffer;
	Matrix view;

	float moveSpeed = 50.0f;
	float rotSpeed = 10.0f;

};