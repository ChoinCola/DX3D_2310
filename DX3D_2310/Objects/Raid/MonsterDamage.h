#pragma once

class MonsterDamage : public MonsterAction
{
public:
	MonsterDamage(TopViewMonster* input);
	~MonsterDamage();

	// MonsterAction을(를) 통해 상속됨
	void Update() override;
	void Start() override;
};