#pragma once

class Coin : public Item
{
public:
	Coin(Vector3 Pos);
	~Coin();
	// Item��(��) ���� ��ӵ�
	void Update() override;
	void Render() override;
	void GetItem(ShootingPlayer* player) override;
};