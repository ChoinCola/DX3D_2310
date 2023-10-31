#pragma once

class ShootingPlayer : public SphereCollider
{
public:
	ShootingPlayer(Vector3 pos);
	~ShootingPlayer();

	void Update();
	void PostRender();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	Vector3 GetFrontPos() { return Frontpos->GetLocalPosition(); }
private:
	void Move();
private:
	float moveSpeed = 5;
	float rotSpeed = 5;
	float Uppos = 0;

	Quad* Frontpos;
	Terrain* terrain;

	Quad* cursor;
};