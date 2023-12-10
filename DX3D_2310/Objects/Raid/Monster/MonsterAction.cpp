#include "Framework.h"

// MonsterAction 클래스의 Move 함수 구현
void MonsterAction::Move(Vector3 direction, float MoveSpeed, float rotSpeed)
{
    // 몬스터를 주어진 방향으로 이동시키고 주어진 속도로 회전
    monster->Translate(direction.GetNormalized() * MoveSpeed * DELTA);
    LookAtRotate(direction, rotSpeed);
}

// MonsterAction 클래스의 LookAtRotate 함수 구현
void MonsterAction::LookAtRotate(Vector3 direction, float rotSpeed)
{
    // 몬스터의 현재 전방 벡터와 목표 방향 벡터 간의 외적 계산
    Vector3 cross = Vector3::Cross(monster->GetForward(), direction);

    // 외적 결과에 따라 몬스터를 회전시킴
    if (cross.y < FLT_EPSILON)
        monster->Rotate(Vector3::Up() * rotSpeed * DELTA);
    else if (cross.y > -FLT_EPSILON)
        monster->Rotate(Vector3::Down() * rotSpeed * DELTA);
}