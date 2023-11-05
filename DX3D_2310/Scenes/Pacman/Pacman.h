#pragma once

class Pacman : public Scene
{
public:
	Pacman();
	~Pacman();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
private:
	void Render_terrain();
	void SetItem();
	void CollisionChack();
	void WinChack();
private:
	list<Item*> itemFild;
	SamplerState* Settile;
	Terrain* ceiling;
	Terrain* terrain;
	ShootingPlayer* player;
	Quad* DefeatBanner;
	Quad* WinBanner;

	int MaxCoinCount = 0;
	int CountCoin	 = 0;
	int CountBullet	 = 0;
	bool Playnow = true;
};