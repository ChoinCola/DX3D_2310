#pragma once

class Bulit : public Item
{
public:
	Bulit(Vector3 Pos);
	~Bulit();
	// Item을(를) 통해 상속됨
	void Update() override;
	void Render() override;
	void GetItem(ShootingPlayer* player) override;
};