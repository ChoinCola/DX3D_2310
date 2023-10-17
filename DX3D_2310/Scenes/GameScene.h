#pragma once

class GameScene : public Scene
{
private:
	const int MAX_SIZE = 3;

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
	unordered_map<int, GameBox*> GameBord;
	GameBox* MovePoint = nullptr;

private:
	void Move();
	void ChackMove(GameBox* nowBox);
	void BoxUP(GameBox* nowBox);
	void BoxDOWN(GameBox* nowBox);
	void BoxLEFT(GameBox* nowBox);
	void BoxRIGHT(GameBox* nowBox);
	void BoxMIX();
};
