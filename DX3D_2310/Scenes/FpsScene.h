#pragma once
class FpsScene : public Scene
{
public:
	FpsScene();
	~FpsScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	void ChackBall();
	void MakeBallList();
	void MoveBallList();
	void PosResetBall(Ball*& input);

private:
	SphereCollider* CamCollider;
	Terrain* terrain;
	list<Ball*> Balllist;
	random_device rd;

	int listSize	= 0;
	int listMaxSize	= 0;


	int Wincount = 20;
	UINT Hitcount = 0;
	float Ball_speed = 0;
	int HP = 10;

	bool Start = false;
	bool End = false;
};