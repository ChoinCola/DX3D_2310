#pragma once

class TopViewGame : public Scene
{
public:
	TopViewGame();
	~TopViewGame();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	TopPlayer* player;
	Map* map;
};