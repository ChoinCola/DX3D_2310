#pragma once

class Ball : public SphereCollider , public Sphere
{
public:
	Ball(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);
	~Ball();

	void Update();
	void Render();
	void GUIRneder();
};