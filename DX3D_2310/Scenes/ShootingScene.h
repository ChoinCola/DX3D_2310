#pragma once

class ShootingScene : public Scene
{
public:
	ShootingScene();
	~ShootingScene();


	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;
	
private:
	Terrain* terrain;
	ShootingPlayer* player;
};