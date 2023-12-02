#pragma once
// TopViewMonster 클래스는 Object를 상속받아 Object 기본 동작을 정의합니다.
class Grass : public Object
{
private:
    // 나무의 상태를 표현하는 열거형
    enum ActionState
    {
        IDLE, // 기본 상태
        SHACK,  // 흔들림 상태
    };

public:
    // 생성자 - 나무의 초기 설정을 수행하며 Transform을 전달 받습니다.
    Grass(Transform* transform);

    // 소멸자 - 나무 관련 자원을 정리합니다.
    ~Grass();

    // 나무의 업데이트를 처리하는 함수
    void Update();

    // 나무를 렌더링하는 함수
    void Render();

    // 나무의 정보를 GUI로 렌더링하는 함수
    void GUIRender();

    // 나무의 행동 상태를 설정하는 함수
    void SetAction(UINT state);
private:
    // 나무의 순찰 동작을 처리하는 함수
    void idle();

    // 나무의 추적 동작을 처리하는 함수
    void Shake();

    // 나무의 다양한 행동을 생성하는 함수
    void CreateActions();

private:
    // Object의 3D 모델 Transform
    Transform* meshTransform;

    // 나무의 추적 대상(Transform)
    Transform* target = nullptr;

    // 몬스터의 현재 행동 상태
    ActionState curState = IDLE;
};