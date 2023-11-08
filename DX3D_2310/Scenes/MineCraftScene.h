#pragma once

class MinceCraftScene : public Scene
{
public:
	MinceCraftScene();
	~MinceCraftScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void CollisionandUp();

private:
	Steve* player;
	//Player* player;
};