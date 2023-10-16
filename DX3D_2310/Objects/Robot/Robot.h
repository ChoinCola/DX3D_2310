#pragma once
class Robot : public Transform
{
public:
	Robot(const Float3 pos, Float3 size);
	~Robot();

	Transform* GetTransform() { return Body1->GetSelf(); }

	void Update();
	void Render();
private:
	vector<Cube> part;

	Cube* Head;
	Cube* Body1;
	Cube* Body2;
	Cube* Body3;

	Cube* LLeg;
	Cube* RLeg;

	Cube* Rshoulder;
	Cube* RArm;

	Cube* Lshoulder;
	Cube* LArm;

	float sinnow = 0;
};