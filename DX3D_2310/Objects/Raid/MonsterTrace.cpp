#include "Framework.h"

// MonsterTrace Ŭ���� ������
MonsterTrace::MonsterTrace(TopViewMonster* monster)
    : MonsterAction(monster)
{
}

// MonsterAction�� ���� ��ӵ� ���� �ൿ ������Ʈ �Լ�
void MonsterTrace::Update()
{
    // ���� �������� ���� ���� ���
    Vector3 direction = target->GetLocalPosition() - monster->GetLocalPosition();

    // ���� �������� �̵� �� ȸ��
    Move(direction, 5, 10);
}

// ���� �ൿ�� ���۵� �� ȣ��Ǵ� �Լ�
void MonsterTrace::Start()
{
    // MonsterManager���� ����� ������ ����
    target = MonsterManager::Get()->GetTarget();
}