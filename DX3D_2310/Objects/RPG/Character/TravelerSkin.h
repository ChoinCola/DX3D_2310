#pragma once
enum Movingtype
{
	Idle,
	Front,
	Left,
	Right,
	Back
};

class TravelerSkin : public ModelAnimator
{
public:
	TravelerSkin(Collider* playerDefalt);
	~TravelerSkin() = default;

	void Update() 
	{
		SetLocalPosition(DefaltPosition->GetLocalPosition() - Vector3(0, 1, 0));
		SetLocalRotation(Vector3(0, DefaltPosition->GetLocalRotation().y + 135, 0));
		__super::Update();
	}
	void Render() { __super::Render(); };
	void GUIRender() { __super::GUIRender(); };

	void SetAnimation(UINT input);
private:
	void MoveFront();
	void MoveLeft();
	void MoveRight();
	void MoveBack();

private:
	Collider* DefaltPosition;
	UINT nowPos = 0;
	UINT nextPos = 0;
};