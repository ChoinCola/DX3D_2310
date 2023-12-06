#include "Framework.h"
#include "TopViewMonster.h"

// TopViewMonster Ŭ���� ������
TopViewMonster::TopViewMonster(Transform* transform, ModelAnimatorInstancing* instancing, UINT num)
    : meshTransform(transform), index(num), instancing(instancing)
{
    // ���޹��� Transform�� �θ�� �����ϰ� �ʿ��� �ʱ�ȭ ����
    transform->SetParent(this);
    transform->SetTag("TopViewMonster");
    transform->Load();

    // ���ʹ� ó������ ��Ȱ��ȭ ���·� ����
    isActive = false;

    motion = instancing->GetMotion(index);
    totalEvent.resize(instancing->GetClipSize());
    eventIters.resize(instancing->GetClipSize());


    // �ʱ� �ൿ ���¸� PATROL�� ����
    SetEvent(HIT, bind(&TopViewMonster::EndDamage, this), 0.8f);
    SetEvent(ATK, bind(&TopViewMonster::EndAttack, this), 0.8f);
    // ������ �پ��� �ൿ�� �����ϴ� �Լ� ȣ��
    CreateActions();
    hpBar = new ProgressBar(L"Textures/UI/HPBar/enemy_hp_bar.png", L"Textures/UI/HPBar/enemy_hp_bar_BG.png");
    hpBarOffset.y = 2.0f;
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
    ExcuteEvent();
    if (isActive == true) DeadTime = 0;
    DeadObejctDelete();
    SetHPBar();
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

void TopViewMonster::PostRender()
{
    hpBar->Render();
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
    curHP -= input;

    hpBar->SetAmount(curHP / maxHP);
    if (maxHP <= 0)
    {
        curState = DIE;
        maxHP = 100;
    }
}

void TopViewMonster::Play(AnimState clip)
{
    MonsterManager::Get()->Play(index, clip);

    this->animState = clip;
    eventIters[clip] = totalEvent[clip].begin();
}

void TopViewMonster::SetEvent(int clip, Event event, float timeRatio)
{
    if (totalEvent[clip].count(timeRatio) > 0)
        return;

    totalEvent[clip][timeRatio] = event;
}

void TopViewMonster::ExcuteEvent()
{
    int clip = curState;

    if (totalEvent.size() <= clip) return;
    if (totalEvent[clip].empty()) return;
    if (eventIters[clip] == totalEvent[clip].end()) return;

    // ��� �������� ����
    float ratio = motion->runningTime / motion->duration;

    if (eventIters[clip]->first > ratio) return;

    eventIters[clip]->second();
    eventIters[clip]++;

}

// ������ �ൿ ���¸� üũ�ϴ� �Լ�
void TopViewMonster::CheckAction()
{
    if (curState == DEAD) return;
    if (curState == HIT) return;

    SetColor(Float4(0, 1, 0, 1));
    // ����� �����Ǿ� ���� �ʴٸ� MonsterManager���� ����� ������
    if (target == nullptr)
        target = MonsterManager::Get()->GetTarget();

    // ���Ϳ� ��� ���� �Ÿ� ���
    float distance = (localPosition - target->GetLocalPosition()).Length();

    // �Ÿ��� ���� �ൿ ���� ����
    if (distance < ATTACK_RANGE)
        SetAction(ATTACK);
    else if (distance < TRACE_RANGE)
        SetAction(TRACE);
    else if (distance >= 10)
        SetAction(PATROL);
}

void TopViewMonster::SetHPBar()
{
    Vector3 screenPos = CAM->WorldToScreen(localPosition + hpBarOffset);

    hpBar->SetLocalPosition(screenPos);
    hpBar->UpdateWorld();
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
    actions.push_back(new MonsterDamage(this));
}

void TopViewMonster::EndDamage()
{
    SetAction(PATROL);
}

void TopViewMonster::EndAttack()
{
    SetAction(PATROL);
}

void TopViewMonster::DeadObejctDelete()
{
    if (DeadTime == DELETETIME)
        isActive = false;
    DeadTime += DELTA;
}
