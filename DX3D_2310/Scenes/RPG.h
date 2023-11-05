#pragma once

class RPG : public Scene
{
public:
	RPG();
	~RPG();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:

	list<Item*> itemlist;
	Player* player;
	Terrain* terrain;
};