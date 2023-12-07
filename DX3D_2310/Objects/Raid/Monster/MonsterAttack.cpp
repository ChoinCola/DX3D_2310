#include "Framework.h"
#include "MonsterAttack.h"

MonsterAttack::MonsterAttack(TopViewMonster* monster)
    : MonsterAction(monster)
{
    monster->SetEvent(TopViewMonster::ATK,
        bind(&MonsterAttack::SendDamage, this), 0.5f);
}

void MonsterAttack::Update()
{
}

void MonsterAttack::Start()
{
    // MonsterManager에서 대상을 가져와 설정
    target = MonsterManager::Get()->GetTarget();
    monster->Play(TopViewMonster::ATK);
}

void MonsterAttack::SendDamage()
{
    ((TopviewTraveler*)target)->Hit(attackPower);
}
