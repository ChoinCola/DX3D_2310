#pragma once

class Bullet: public Item
{
public:
	Bullet(Vector3 Pos);
	~Bullet();
	// Item��(��) ���� ��ӵ�
	void Update() override;
	void Render() override;
	void GetItem(int& player) override;
private:
	void Lightsetting();
private:
	int light;
};