#pragma once

class Seller : public SphereCollider
{
private:
	const float GRAVITY = 98.0f;

public:
	Seller();
	~Seller();

	void Update();
	void GUIRender();
	void PostRender();
	void Render();

	void ChackOpenUi(void* Collider);

private:
	ShopUI* ui;
	Model* model;
	bool Onlyone = true;
};