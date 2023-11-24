#pragma once

class Groot : public CapsuleCollider
{

private:
	enum ActionState
	{
		IDLE, RUN_F, RUN_R, RUN_L,
		ATTACK
	};

public:
	Groot();
	~Groot();

	void Update();
	void Render();
	void GUIRender();

private:
	void Attack();
	void EndAttack();

	void SetState(ActionState state);
	void ReadClips();

private:
	ModelAnimator* bodyMesh;
	Model* sword;

	Transform* rightHand;
	ActionState curstate;
};