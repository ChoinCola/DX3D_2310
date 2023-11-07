#pragma once
class Player : public SphereCollider
{
public:
	Player(Vector3 pos = { 0, 0, 0 });
	~Player();
	
	void Update();
	void Render();
	void GUIRender();

	void UIRender();
	void SetIsUI();

	void SetIsItemBox();
	void SetTerrain(Terrain* terrain);
	void GetItem(Item& item);
	void OutItem(Item& item);

	const Ray& GetDownRay() { return DownRay; }
	bool& IsJump() { return Isjump; }
	float GetMoveSpeed() { return moveSpeed; }

private:
	void Move();
	void Load();
	void Save();
	void OpenItemBox();

private:
	ItemBox* itemBox = nullptr;
	Model* Character = nullptr;
	Terrain* terrain = nullptr;
	Ray DownRay;

	int lightnum	 = 0;
	int moveSpeed	 = 0;
	int rotSpeed	 = 0;

	float Uppos = 0;
	float weight = 8;

	float JumpSpeed = 12.8;
	float MaxJumpHight = 4;
	float JumpHight = 0;

	bool IsOpenItemBox = true;
	bool IsRenderUI = false;
	bool isInplay = false;
	bool Isjump = false;
};