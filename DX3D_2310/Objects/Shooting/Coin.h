#pragma once

class Coin : public Item
{
public:
	Coin(Vector3 Pos);
	~Coin();
	// Item을(를) 통해 상속됨
	void Update() override;
	void Render() override;
	void GetItem(int& player) override;

	int GetMaxCoinCount() { return Coincount; }
private:
	static int Coincount;
};
