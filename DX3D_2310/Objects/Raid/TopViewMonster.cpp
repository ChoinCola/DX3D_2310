#include "Framework.h"

// TopViewMonster Ŭ���� ������
TopViewMonster::TopViewMonster(Transform* transform) : meshTransform(transform)
{
    // ���޹��� Transform�� �θ�� �����ϰ� �ʿ��� �ʱ�ȭ ����
    transform->SetParent(this);
    transform->SetTag("TopViewMonster");
    transform->Load();

    // ���ʹ� ó������ ��Ȱ��ȭ ���·� ����
    isActive = false;

    // ������ �پ��� �ൿ�� �����ϴ� �Լ� ȣ��
    CreateActions();

    // �ʱ� �ൿ ���¸� PATROL�� ����
    SetAction(PATROL);
}

// TopViewMonster Ŭ���� �Ҹ���
TopViewMonster::~TopViewMonster()
{
    // ������ �Ҹ��ڿ����� �߰����� ���� �۾��� �ʿ����� ����
}

// ������ ������Ʈ�� ó���ϴ� �Լ�
void TopViewMonster::Update()
{
    // ���Ͱ� ��Ȱ��ȭ ������ ��� ������Ʈ�� �������� ����
    if (!IsActive()) return;

    // �θ� Ŭ������ UpdateWorld �Լ� ȣ��
    UpdateWorld();
}

// ���͸� �������ϴ� �Լ�
void TopViewMonster::Render()
{
    // ���Ͱ� ��Ȱ��ȭ ������ ��� �������� �������� ����
    if (!IsActive()) return;

    // ���� �ൿ ���¿� ���� �ش��ϴ� �ൿ ����
    CheckAction();
    actions[curState]->Update();

    // CapsuleCollider�� Render �Լ� ȣ��
    __super::Render();
}

// ������ ������ GUI�� �������ϴ� �Լ�
void TopViewMonster::GUIRender()
{
    // ���Ͱ� ��Ȱ��ȭ ������ ��� GUI �������� �������� ����
    if (!IsActive()) return;

    // CapsuleCollider�� GUIRender �Լ� ȣ��
    __super::GUIRender();
}

// ������ �ൿ ���¸� üũ�ϴ� �Լ�
void TopViewMonster::CheckAction()
{
    // ����� �����Ǿ� ���� �ʴٸ� MonsterManager���� ����� ������
    if (target == nullptr)
        target = MonsterManager::Get()->GetTarget();

    // ���Ϳ� ��� ���� �Ÿ� ���
    float distance = (localPosition - target->GetLocalPosition()).Length();

    // �Ÿ��� ���� �ൿ ���� ����
    if (distance < 10)
        SetAction(TRACE);
    else if (distance >= 10)
        SetAction(PATROL);
}

// ������ �ൿ ���¸� �����ϴ� �Լ�
void TopViewMonster::SetAction(ActionState state)
{
    // ���� ���¿� ������ ���·� �����Ǹ� ����
    if (curState == state) return;

    // ���� ���� �� ���� ���� ������Ʈ �� �ش� ������ �ൿ ����
    curState = state;
    actions[state]->Start();
}

// ������ ���� ������ ó���ϴ� �Լ� (���� ���� �ʿ�)
void TopViewMonster::Patrol()
{
    // ���� ���� ����
}

// ������ ���� ������ ó���ϴ� �Լ� (���� ���� �ʿ�)
void TopViewMonster::Trace()
{
    // ���� ���� ����
}

// ������ ���� ������ ó���ϴ� �Լ� (���� ���� �ʿ�)
void TopViewMonster::Attack()
{
    // ���� ���� ����
}

// ������ �پ��� �ൿ�� �����ϴ� �Լ�
void TopViewMonster::CreateActions()
{
    // ���� �� ���� ������ ���� �ൿ ��ü���� �����ϰ� ���Ϳ� �߰�
    actions.push_back(new MonsterPatrol(this));
    actions.push_back(new MonsterTrace(this));
}