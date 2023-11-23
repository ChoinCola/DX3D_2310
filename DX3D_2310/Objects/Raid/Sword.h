#pragma once

class Sword : public Model
{
public:
	Sword();
	~Sword();

	void Update();
	void Render();

	BoxCollider* GetCollider() { return collider; }

private:
	BoxCollider* collider;
};