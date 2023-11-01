#pragma once

class Bullet: public Item
{
public:
	Bullet(Vector3 Pos);
	~Bullet();
	// Item을(를) 통해 상속됨
	void Update() override;
	void Render() override;
	void GetItem(int& player) override;
private:
	void Lightsetting();
private:
	int light;
};