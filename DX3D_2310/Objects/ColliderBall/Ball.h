#pragma once

class Ball : public SphereCollider
{
public:
	Ball(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);
	~Ball();

	void Update();
	void Render();
	void GUIRneder();
	float Getradius() { return radius; }
	Sphere* GetSphere() { return sphere; }
private:
	float radius;
	Sphere* sphere;
};