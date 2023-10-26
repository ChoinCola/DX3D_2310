#pragma once
class Robot : public Transform
{
public:
	Robot(const Float3 pos, float size);
	~Robot();

	Transform* GetTransform() { return Body1->GetSelf(); }

	void Update();
	void Render();
	void GUIRender();
private:
	void Move();
	//void MoveFonrtStep();
	//void MoveSideStep();
	//void MoveBackStep();
	//void MoveJumpStep();

private:
	vector<Cube*> part;

	Cube* BasePos;

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
	float moveSpeed = 1;
	float rotSpeed = 1;

	bool IsInputF2 = false;
	bool IsDiffuseSet	= true;
	bool IsDiffuseSetOld= true;

	bool IsSpecularMapSet	= true;
	bool IsSpecularMapSetOld= true;

	bool IsNormalMapSet		= true;
	bool IsNormalMapSetOld	= true;
};