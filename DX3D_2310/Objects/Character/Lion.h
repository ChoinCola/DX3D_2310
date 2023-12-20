#pragma once

class Lion : public BoxCollider
{
private:
	enum State
	{
		IDLE, RUN
	};

public:
	Lion();
	~Lion();

	void Update();
	void Render();
	void GUIRender();
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

private:
	void Control();
	void Move();
	void Rotate();

	void SetState(State state);

private:
	ModelAnimator* model;
	Terrain* terrain = nullptr;
	
	float moveSpeed = 10.0f, rotSpeed = 5.0f;

	State curState = IDLE;

	Vector3 velocity, destPos;
	vector<Vector3> path;
};