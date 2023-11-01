#pragma once

class ShootingPlayer : public SphereCollider
{
public:
	ShootingPlayer(Vector3 pos);
	~ShootingPlayer();

	void Update();
	void PostRender();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

private:
	void Move();
	void SetLight();
private:
	float moveSpeed = 5;
	float rotSpeed = 5;
	float Uppos = 0;
	LightBuffer::Light* light;

	Terrain* terrain;

	Quad* cursor;
};