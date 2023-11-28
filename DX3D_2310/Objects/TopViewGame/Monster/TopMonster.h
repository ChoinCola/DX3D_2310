#pragma once

class TopMonster : public CapsuleCollider
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
	TopMonster();
	~TopMonster();

	void Update();
	void Render();
	void GUIRender();

	CapsuleCollider* GetCollider() { return this; }

private:
	void Move();
	void UpdateUppos();
	void SetOnTerrain(Terrain* input);

private:
	ModelAnimator* model;
	Terrain* Onterrain = nullptr;

	UINT state;
};