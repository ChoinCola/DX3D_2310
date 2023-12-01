#include "Framework.h"

// MonsterTrace 클래스 생성자
MonsterTrace::MonsterTrace(TopViewMonster* monster)
    : MonsterAction(monster)
{
}

// MonsterAction을 통해 상속된 추적 행동 업데이트 함수
void MonsterTrace::Update()
{
    // 추적 대상까지의 방향 벡터 계산
    Vector3 direction = target->GetLocalPosition() - monster->GetLocalPosition();

    // 계산된 방향으로 이동 및 회전
    Move(direction, 5, 10);
}

// 추적 행동이 시작될 때 호출되는 함수
void MonsterTrace::Start()
{
    // MonsterManager에서 대상을 가져와 설정
    target = MonsterManager::Get()->GetTarget();
}