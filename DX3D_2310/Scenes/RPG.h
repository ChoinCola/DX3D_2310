#pragma once

class RPG : public Scene
{
public:
	RPG();
	~RPG();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void CollisionandUp();

private:

	list<Item*> itemlist;
	Player* player;
	Terrain* terrain;
};