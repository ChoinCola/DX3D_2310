#include "Framework.h"

// MonsterPatrol Ŭ���� ������
MonsterPatrol::MonsterPatrol(TopViewMonster* monster)
    : MonsterAction(monster)
{
}

// MonsterAction�� ���� ��ӵ� ���� �ൿ ������Ʈ �Լ�
void MonsterPatrol::Update()
{
    // ��ǥ ��ġ������ ���� ���� ���
    Vector3 direction = destPos - monster->GetLocalPosition();
    direction.y = 0;

    // ���� �������� �̵� �� ȸ��
    Move(direction, moveSpeed, rotSpeed);

    // ��ǥ ��ġ�� �������� �� ���ο� ��ǥ ��ġ ����
    if (direction.Length() < 1.0f)
        SetDestPos();
}

// ���� �ൿ�� ���۵� �� ȣ��Ǵ� �Լ�
void MonsterPatrol::Start()
{
    // �ʱ� ��ǥ ��ġ ����
    SetDestPos();
}

// ���� ��ǥ ��ġ�� �����ϴ� �Լ�
void MonsterPatrol::SetDestPos()
{
    // �������� ������ �����Ͽ� ��ǥ ��ġ ����
    Vector3 direction;
    direction.x = MATH->Random(-1.0f, 1.0f);
    direction.z = MATH->Random(-1.0f, 1.0f);

    destPos = monster->GetLocalPosition() + direction.GetNormalized() * RANGE;
}