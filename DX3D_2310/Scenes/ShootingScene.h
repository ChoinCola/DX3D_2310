#pragma once

class ShootingScene : public Scene
{
public:
	ShootingScene();
	~ShootingScene();


	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
	
private:
	Terrain* terrain;
	ShootingPlayer* player;
};