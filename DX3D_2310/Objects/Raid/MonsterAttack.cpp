#include "Framework.h"
#include "MonsterAttack.h"

MonsterAttack::MonsterAttack(TopViewMonster* monster)
    : MonsterAction(monster)
{
}

void MonsterAttack::Update()
{
    // 목표 위치까지의 방향 벡터 계산
    Vector3 direction = target->GetLocalPosition() - monster->GetLocalPosition();
    direction.y = 0;

    // 계산된 방향으로 이동 및 회전
    LookAtRotate(direction, rotSpeed);
}

void MonsterAttack::Start()
{
    // MonsterManager에서 대상을 가져와 설정
    target = MonsterManager::Get()->GetTarget();
}
