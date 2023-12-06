#include "Framework.h"
#include "TopViewMonster.h"

// TopViewMonster 클래스 생성자
TopViewMonster::TopViewMonster(Transform* transform, ModelAnimatorInstancing* instancing, UINT num)
    : meshTransform(transform), index(num), instancing(instancing)
{
    // 전달받은 Transform을 부모로 설정하고 필요한 초기화 수행
    transform->SetParent(this);
    transform->SetTag("TopViewMonster");
    transform->Load();

    // 몬스터는 처음에는 비활성화 상태로 시작
    isActive = false;

    motion = instancing->GetMotion(index);
    totalEvent.resize(instancing->GetClipSize());
    eventIters.resize(instancing->GetClipSize());


    // 초기 행동 상태를 PATROL로 설정
    SetEvent(HIT, bind(&TopViewMonster::EndDamage, this), 0.8f);
    SetEvent(ATK, bind(&TopViewMonster::EndAttack, this), 0.8f);
    // 몬스터의 다양한 행동을 생성하는 함수 호출
    CreateActions();
    hpBar = new ProgressBar(L"Textures/UI/HPBar/enemy_hp_bar.png", L"Textures/UI/HPBar/enemy_hp_bar_BG.png");
    hpBarOffset.y = 2.0f;
}

// TopViewMonster 클래스 소멸자
TopViewMonster::~TopViewMonster()
{
    // 몬스터의 소멸자에서는 추가적인 정리 작업이 필요하지 않음
}

// 몬스터의 업데이트를 처리하는 함수
void TopViewMonster::Update()
{
    // 몬스터가 비활성화 상태인 경우 업데이트를 수행하지 않음
    if (!IsActive()) return;

    // 부모 클래스의 UpdateWorld 함수 호출
    UpdateWorld();
    ExcuteEvent();
    if (isActive == true) DeadTime = 0;
    DeadObejctDelete();
    SetHPBar();
}

// 몬스터를 렌더링하는 함수
void TopViewMonster::Render()
{
    // 몬스터가 비활성화 상태인 경우 렌더링을 수행하지 않음
    if (!IsActive()) return;

    // 현재 행동 상태에 따라 해당하는 행동 수행
    CheckAction();
    actions[curState]->Update();

    // CapsuleCollider의 Render 함수 호출
    __super::Render();
}

void TopViewMonster::PostRender()
{
    hpBar->Render();
}

// 몬스터의 정보를 GUI로 렌더링하는 함수
void TopViewMonster::GUIRender()
{
    // 몬스터가 비활성화 상태인 경우 GUI 렌더링을 수행하지 않음
    if (!IsActive()) return;

    // CapsuleCollider의 GUIRender 함수 호출
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

    // 모션 진행중인 비율
    float ratio = motion->runningTime / motion->duration;

    if (eventIters[clip]->first > ratio) return;

    eventIters[clip]->second();
    eventIters[clip]++;

}

// 몬스터의 행동 상태를 체크하는 함수
void TopViewMonster::CheckAction()
{
    if (curState == DEAD) return;
    if (curState == HIT) return;

    SetColor(Float4(0, 1, 0, 1));
    // 대상이 설정되어 있지 않다면 MonsterManager에서 대상을 가져옴
    if (target == nullptr)
        target = MonsterManager::Get()->GetTarget();

    // 몬스터와 대상 간의 거리 계산
    float distance = (localPosition - target->GetLocalPosition()).Length();

    // 거리에 따라 행동 상태 설정
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

// 몬스터의 행동 상태를 설정하는 함수
void TopViewMonster::SetAction(ActionState state)
{
    // 현재 상태와 동일한 상태로 설정되면 무시
    if (curState == state) {
        ChangeMotion = false;
        return;
    }

    // 상태 변경 시 현재 상태 업데이트 및 해당 상태의 행동 시작
    curState = state;
    actions[state]->Start();
}

// 몬스터의 순찰 동작을 처리하는 함수 (추후 구현 필요)
void TopViewMonster::Patrol()
{
    // 순찰 동작 구현
}

// 몬스터의 추적 동작을 처리하는 함수 (추후 구현 필요)
void TopViewMonster::Trace()
{
    // 추적 동작 구현
}

// 몬스터의 공격 동작을 처리하는 함수 (추후 구현 필요)
void TopViewMonster::Attack()
{
    // 공격 동작 구현
}

// 몬스터의 다양한 행동을 생성하는 함수
void TopViewMonster::CreateActions()
{
    // 순찰 및 추적 동작을 담은 행동 객체들을 생성하고 벡터에 추가
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
