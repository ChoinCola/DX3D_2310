#pragma once

class MonsterDamage : public MonsterAction
{
public:
	MonsterDamage(TopViewMonster* input);
	~MonsterDamage();

	// MonsterAction��(��) ���� ��ӵ�
	void Update() override;
	void Start() override;
};