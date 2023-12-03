#include "Framework.h"
#include "TopViewMonster.h"

// TopViewMonster Ŭ���� ������
TopViewMonster::TopViewMonster(Transform* transform, UINT num) : meshTransform(transform), monsternumber(num)
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
    if (isActive == true) DeadTime = 0;
    DeadObejctDelete();
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

void TopViewMonster::Hit(float input)
{
    HP -= input;
    if (HP <= 0)
    {
        curState = DEAD;
        HP = 100;
    }
}

// ������ �ൿ ���¸� üũ�ϴ� �Լ�
void TopViewMonster::CheckAction()
{
    if (curState == DEAD) return;


    if (FireBallManager::Get()->ChackCollision(this))
        SetAction(HIT);

    if (curState == HIT) return;
    SetColor(Float4(0, 1, 0, 1));
    // ����� �����Ǿ� ���� �ʴٸ� MonsterManager���� ����� ������
    if (target == nullptr)
        target = MonsterManager::Get()->GetTarget();

    // ���Ϳ� ��� ���� �Ÿ� ���
    float distance = (localPosition - target->GetLocalPosition()).Length();

    // �Ÿ��� ���� �ൿ ���� ����
    if (distance < 1)
        SetAction(ATTACK);
    else if (distance < 10)
        SetAction(TRACE);
    else if (distance >= 10)
        SetAction(PATROL);

    if (Attime > ATTACKDELAY) {
        Attime -= ATTACKDELAY;
    }
    else
        Attime += DELTA;
}

// ������ �ൿ ���¸� �����ϴ� �Լ�
void TopViewMonster::SetAction(ActionState state)
{
    // ���� ���¿� ������ ���·� �����Ǹ� ����
    if (curState == state) {
        ChangeMotion = false;
        return;
    }

    // ���� ���� �� ���� ���� ������Ʈ �� �ش� ������ �ൿ ����
    curState = state;
    actions[state]->Start();
    ChangeMotion = true;
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
    actions.push_back(new MonsterAttack(this));
    actions.push_back(new MonsterHit(this));
}

void TopViewMonster::DeadObejctDelete()
{
    if (DeadTime == DELETETIME)
        isActive = false;
    DeadTime += DELTA;
}
