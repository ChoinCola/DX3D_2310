#pragma once

class TopPlayer : public CapsuleCollider
{
	enum AnimState
	{
		IDLE,
		MOVE,
		ATTACK,
		HIT,
		DEAD
	};

public:
	TopPlayer();
	~TopPlayer();

	void Update();
	void Render();
	void GUIRender();

	CapsuleCollider* GetCollider() { return this; }

private:
	void CamMove();
	void Move();
	void UpdateUppos();
	void SetOnTerrain(Terrain* input);

private:
	ModelAnimator* model;
	Terrain* Onterrain = nullptr;

	AnimState state;

	float moveSpeed = 10.0f;
	float rotSpeed = 2.0f;
	float accelation = 2.0f;
	float deceleration = 10.0f;
	Vector3 veloctiy;
};