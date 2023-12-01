#pragma once

// TopViewMonster 클래스는 CapsuleCollider를 상속받아 몬스터의 기본 동작을 정의합니다.
class TopViewMonster : public CapsuleCollider
{
private:
    // 몬스터의 상태를 표현하는 열거형
    enum ActionState
    {
        PATROL, // 순찰 상태
        TRACE,  // 추적 상태
        ATTACK, // 공격 상태
        NONE    // 상태 없음
    };

public:
    // 생성자 - 몬스터의 초기 설정을 수행하며 Transform을 전달 받습니다.
    TopViewMonster(Transform* transform);

    // 소멸자 - 몬스터 관련 자원을 정리합니다.
    ~TopViewMonster();

    // 몬스터의 업데이트를 처리하는 함수
    void Update();

    // 몬스터를 렌더링하는 함수
    void Render();

    // 몬스터의 정보를 GUI로 렌더링하는 함수
    void GUIRender();

private:
    // 몬스터의 현재 행동 상태를 결정하는 함수
    void CheckAction();

    // 몬스터의 행동 상태를 설정하는 함수
    void SetAction(ActionState state);

    // 몬스터의 순찰 동작을 처리하는 함수
    void Patrol();

    // 몬스터의 추적 동작을 처리하는 함수
    void Trace();

    // 몬스터의 공격 동작을 처리하는 함수
    void Attack();

    // 몬스터의 다양한 행동을 생성하는 함수
    void CreateActions();

private:
    // 몬스터의 3D 모델 Transform
    Transform* meshTransform;

    // 몬스터의 추적 대상(Transform)
    Transform* target = nullptr;

    // 몬스터의 이동 속도
    float moveSpeed = 5.0f;

    // 몬스터의 회전 속도
    float rotSpeed = 10.0f;

    // 몬스터의 현재 속도 벡터
    Vector3 velocity;

    // 몬스터의 현재 행동 상태
    ActionState curState = NONE;

    // 몬스터의 다양한 행동을 담는 벡터
    vector<MonsterAction*> actions;
};