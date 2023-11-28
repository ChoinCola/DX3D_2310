#pragma once

class TopViewGame : public Scene
{
public:
	TopViewGame();
	~TopViewGame();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	TopPlayer* player;
	Map* map;
};