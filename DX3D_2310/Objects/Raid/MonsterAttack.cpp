#include "Framework.h"
#include "MonsterAttack.h"

MonsterAttack::MonsterAttack(TopViewMonster* monster)
    : MonsterAction(monster)
{
}

void MonsterAttack::Update()
{
    // ��ǥ ��ġ������ ���� ���� ���
    Vector3 direction = target->GetLocalPosition() - monster->GetLocalPosition();
    direction.y = 0;

    // ���� �������� �̵� �� ȸ��
    LookAtRotate(direction, rotSpeed);
}

void MonsterAttack::Start()
{
    // MonsterManager���� ����� ������ ����
    target = MonsterManager::Get()->GetTarget();
}
