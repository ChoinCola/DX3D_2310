#pragma once

class Sword : public Model
{
public:
	Sword();
	~Sword();

	void Update();
	void Render();

	float GetDamage() { return damage; }
	BoxCollider* GetCollider() { return collider; }

private:
	BoxCollider* collider;
	float damage = 10;
};