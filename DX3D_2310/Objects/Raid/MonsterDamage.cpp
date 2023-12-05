#include "Framework.h"

MonsterDamage::MonsterDamage(TopViewMonster* input) 
	: MonsterAction(input)
{
}

MonsterDamage::~MonsterDamage()
{
}

void MonsterDamage::Update()
{
}

void MonsterDamage::Start()
{
	monster->Play(TopViewMonster::HIT);
}
