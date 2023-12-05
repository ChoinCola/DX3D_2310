#pragma once

class TopViewMonster;

class FireBallManager : public Singleton<FireBallManager>
{
private:
	const float FULL_SIZE = 100;
private:
	friend class Singleton;

	FireBallManager();
	~FireBallManager();

public:
	void Update();
	void Render();
	void GUIRender();

	void Fire(Vector3 pos, Vector3 direction);

	bool IsCollision(Collider* output);

private:
	ModelInstancing* modelInstancing;

	vector<FireBall*> fireBalls;
};