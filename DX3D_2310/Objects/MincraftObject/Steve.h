#pragma once
#include "MouseBag.h"

class Steve : public SphereCollider
{
private:
	const float GRAVITY = 98.0f;
	const float JUMP_POWER = 50.0f;
public:
	Steve();
	~Steve();

	void Update();
	void GUIRender();
	void PostRender();
	void Render();

private:
	void Move();
	void Jump();
	void Control();

private:
	float moveSpeed = 5;
	float rotSpeed = 5;

	float velocity = 0.0f;

	Vector3 Uprotation = {};

	Vector3 prevMousePos;
	POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };

	bool isFree = true;

	MineCraftUI* ui;

	float v[3] = { 0, 1, -5 };
	float r[3];

	Vector3 campos = {};
	Vector3 camrot = {};
};