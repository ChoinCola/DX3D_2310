#pragma once

// TopViewMonster 클래스는 CapsuleCollider를 상속받아 몬스터의 기본 동작을 정의합니다.
class TopViewMonster : public CapsuleCollider
{
public:

    enum AnimState
    {
        WALK, RUN, ATK, HIT, DEAD
    };

    enum ActionState
    {
        PATROL, // 순찰 상태
        TRACE,  // 추적 상태
        ATTACK, // 공격 상태
        DAMAGE,    // 맞는 상태
        DIE,   // 죽는 상태
        NONE    // 상태 없음
    };

private:

    const float TRACE_RANGE = 10.0f;
    const float ATTACK_RANGE = 2.0f;

    const float DELETETIME = 5;
    const float ATTACKDELAY = 1;
    // 몬스터의 상태를 표현하는 열거형

public:
    // 생성자 - 몬스터의 초기 설정을 수행하며 Transform을 전달 받습니다.
    TopViewMonster(Transform* transform, ModelAnimatorInstancing* instancing, UINT num);

    // 소멸자 - 몬스터 관련 자원을 정리합니다.
    ~TopViewMonster();

    // 몬스터의 업데이트를 처리하는 함수
    void Update();

    // 몬스터를 렌더링하는 함수
    void Render();
    // 몬스터의 정보를 GUI로 렌더링하는 함수
    void GUIRender();

    void Hit(float input);

    // 몬스터의 행동 상태를 설정하는 함수
    void Play(AnimState clip);
    void SetAction(ActionState state);

    void SetEvent(int clip, Event event, float timeRatio);
    void ExcuteEvent();

    ActionState GetAction() { return curState; }

    UINT Getnumber() { return index; }
    bool NowChangeMotion() { return ChangeMotion; }


private:
    // 몬스터의 현재 행동 상태를 결정하는 함수
    void CheckAction();

    void SetHPBar();

    // 몬스터의 다양한 행동을 생성하는 함수
    void CreateActions();

    void EndDamage();
    void EndAttack();

private:
    // 몬스터의 3D 모델 Transform
    Transform* meshTransform;

    // 몬스터의 추적 대상(Transform)
    Transform* target = nullptr;

    // 몬스터의 번호.
    UINT index = 0;
    // 몬스터의 이동 속도
    float moveSpeed = 5.0f;

    // 몬스터의 회전 속도
    float rotSpeed = 10.0f;

    float maxHP = 100.0f;
    float curHP = 100.0f;
    float DeadTime = 0;
    float Attime = 0;
    bool ChangeMotion = false;
    // 몬스터의 현재 속도 벡터
    Vector3 velocity;

    // 몬스터의 현재 행동 상태
    ActionState curState = NONE;
    // 몬스터의 현재 애니메이션 상태
    AnimState animState = RUN;

    ModelAnimatorInstancing* instancing;
    ModelAnimatorInstancing::Motion* motion;
    
    // 몬스터의 다양한 행동을 담는 벡터
    vector<MonsterAction*> actions;

    vector<map<float, Event>> totalEvent;
    vector<map<float, Event>::iterator> eventIters;

    ProgressBar* hpBar;
    Vector3 hpBarOffset;
};