#pragma once

class GameScene : public Scene
{
private:
	const float MAX_SIZE = 4;

public:
	GameScene();
	~GameScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	unordered_map<float, GameBox*> GameBord;
	GameBox* MovePoint = nullptr;

	Vector3 Swap1, Swap2;
	float Sw1, Sw2;

	float SwapSpeed;
	float nowPos;
	bool Moving = false;
private:
	void Swap(float a2);
	void Move();
	void ChackMove();
	void BoxMIX();
};
