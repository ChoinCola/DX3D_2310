#include "Framework.h"

// MonsterAction Ŭ������ Move �Լ� ����
void MonsterAction::Move(Vector3 direction, float MoveSpeed, float rotSpeed)
{
    // ���͸� �־��� �������� �̵���Ű�� �־��� �ӵ��� ȸ��
    monster->Translate(direction.GetNormalized() * MoveSpeed * DELTA);
    LookAtRotate(direction, rotSpeed);
}

// MonsterAction Ŭ������ LookAtRotate �Լ� ����
void MonsterAction::LookAtRotate(Vector3 direction, float rotSpeed)
{
    // ������ ���� ���� ���Ϳ� ��ǥ ���� ���� ���� ���� ���
    Vector3 cross = Vector3::Cross(monster->GetForward(), direction);

    // ���� ����� ���� ���͸� ȸ����Ŵ
    if (cross.y < FLT_EPSILON)
        monster->Rotate(Vector3::Up() * rotSpeed * DELTA);
    else if (cross.y > -FLT_EPSILON)
        monster->Rotate(Vector3::Down() * rotSpeed * DELTA);
}