#pragma once

class Ealth : public Spher
{
public:
	Ealth(Vector3 pivot);
	~Ealth();
	void Update();
	void Render();
private:
	float AngleRevolution = 0;
};