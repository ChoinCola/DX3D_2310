#pragma once

class Moon : public Spher
{
public:
	Moon(Transform* Parent);
	~Moon();
	void Update();
	void Render();
private:
	float AngleRevolution = 0;
	Transform* parent;
};