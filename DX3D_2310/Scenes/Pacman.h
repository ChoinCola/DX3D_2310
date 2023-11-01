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

private:

	SamplerState* Settile;
	Terrain* ceiling;
	Terrain* terrain;
	ShootingPlayer* player;
};