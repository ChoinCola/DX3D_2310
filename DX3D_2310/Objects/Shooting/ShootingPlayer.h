#pragma once

class ShootingPlayer : public SphereCollider
{
public:
	ShootingPlayer(Vector3 pos);
	~ShootingPlayer();

	void Update();
	void GUIRender();
	void PostRender();
	void Render();
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	float& GetLightBat() { return LightBat; }
private:
	void Move();
	void SetLight();
	void UpdateLightBat();
	float Sigmoid(float x, float k, float x0);
private:
	LightBuffer::Light* light;

	Terrain* terrain;

	Quad* cursor;
	Quad* lightBar;
	POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };

	float moveSpeed = 5;
	float rotSpeed = 5;
	float Uppos = 0;

	float rotateX = 0;

	float LightBat = 100;
	float lightBatdelta = 0;
	bool isFree = true;
};