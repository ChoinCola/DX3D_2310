#include "Framework.h"

// MonsterPatrol 클래스 생성자
MonsterPatrol::MonsterPatrol(TopViewMonster* monster)
    : MonsterAction(monster)
{
}

// MonsterAction을 통해 상속된 순찰 행동 업데이트 함수
void MonsterPatrol::Update()
{
    // 목표 위치까지의 방향 벡터 계산
    Vector3 direction = destPos - monster->GetLocalPosition();
    direction.y = 0;

    // 계산된 방향으로 이동 및 회전
    Move(direction, moveSpeed, rotSpeed);

    // 목표 위치에 도달했을 때 새로운 목표 위치 설정
    if (direction.Length() < 1.0f)
        SetDestPos();
}

// 순찰 행동이 시작될 때 호출되는 함수
void MonsterPatrol::Start()
{
    // 초기 목표 위치 설정
    SetDestPos();
    monster->Play(TopViewMonster::WALK);
}

// 순찰 목표 위치를 설정하는 함수
void MonsterPatrol::SetDestPos()
{
    // 무작위로 방향을 생성하여 목표 위치 설정
    Vector3 direction;
    direction.x = MATH->Random(-1.0f, 1.0f);
    direction.z = MATH->Random(-1.0f, 1.0f);

    destPos = monster->GetLocalPosition() + direction.GetNormalized() * RANGE;
}