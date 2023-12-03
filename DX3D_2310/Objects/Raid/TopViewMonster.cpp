#include "Framework.h"
#include "TopViewMonster.h"

// TopViewMonster 클래스 생성자
TopViewMonster::TopViewMonster(Transform* transform, UINT num) : meshTransform(transform), monsternumber(num)
{
    // 전달받은 Transform을 부모로 설정하고 필요한 초기화 수행
    transform->SetParent(this);
    transform->SetTag("TopViewMonster");
    transform->Load();

    // 몬스터는 처음에는 비활성화 상태로 시작
    isActive = false;

    // 몬스터의 다양한 행동을 생성하는 함수 호출
    CreateActions();

    // 초기 행동 상태를 PATROL로 설정
    SetAction(PATROL);
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
    if (isActive == true) DeadTime = 0;
    DeadObejctDelete();
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
    HP -= input;
    if (HP <= 0)
    {
        curState = DEAD;
        HP = 100;
    }
}

// 몬스터의 행동 상태를 체크하는 함수
void TopViewMonster::CheckAction()
{
    if (curState == DEAD) return;


    if (FireBallManager::Get()->ChackCollision(this))
        SetAction(HIT);

    if (curState == HIT) return;
    SetColor(Float4(0, 1, 0, 1));
    // 대상이 설정되어 있지 않다면 MonsterManager에서 대상을 가져옴
    if (target == nullptr)
        target = MonsterManager::Get()->GetTarget();

    // 몬스터와 대상 간의 거리 계산
    float distance = (localPosition - target->GetLocalPosition()).Length();

    // 거리에 따라 행동 상태 설정
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
    ChangeMotion = true;
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
    actions.push_back(new MonsterHit(this));
}

void TopViewMonster::DeadObejctDelete()
{
    if (DeadTime == DELETETIME)
        isActive = false;
    DeadTime += DELTA;
}
