#pragma once

class Bulit : public Item
{
public:
	Bulit(Vector3 Pos);
	~Bulit();
	// Item��(��) ���� ��ӵ�
	void Update() override;
	void Render() override;
	void GetItem(ShootingPlayer* player) override;
};