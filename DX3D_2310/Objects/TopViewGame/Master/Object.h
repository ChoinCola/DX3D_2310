#pragma once
class Object : public SphereCollider
{
private:
    // 오브젝트 상태를 표현하는 열거형
    enum ActionState
    {
        IDLE, // 기본 상태
        SHACK,  // 흔들림 상태
    };

public:
    // 생성자 - 나무의 초기 설정을 수행하며 Transform을 전달 받습니다.
    Object(Transform* transform) : meshTransform(transform) 
    {   transform->SetParent(this); 
        transform->SetTag("Object");
        transform->Load();
        // 몬스터는 처음에는 비활성화 상태로 시작
        isActive = false;

        // 초기 행동 상태를 Idle로 설정
        SetAction(IDLE);
    };

    // 소멸자 - 나무 관련 자원을 정리합니다.
    ~Object() = default;

    // 나무의 업데이트를 처리하는 함수
    void Update() {     // 몬스터가 비활성화 상태인 경우 업데이트를 수행하지 않음
        if (!IsActive()) return;

        // 부모 클래스의 UpdateWorld 함수 호출
        UpdateWorld();
    };

    // 나무를 렌더링하는 함수
    void Render() {
        // 몬스터가 비활성화 상태인 경우 렌더링을 수행하지 않음
        if (!IsActive()) return;

        // CapsuleCollider의 Render 함수 호출
        __super::Render();
    };

    // 나무의 정보를 GUI로 렌더링하는 함수
    void GUIRender() {

        // 몬스터가 비활성화 상태인 경우 GUI 렌더링을 수행하지 않음
        if (!IsActive()) return;

        // CapsuleCollider의 GUIRender 함수 호출
        __super::GUIRender();
    };


    UINT GetState() { return curState; }

    // 나무의 행동 상태를 설정하는 함수
    void SetAction(UINT state)
    {
        // 현재 상태와 동일한 상태로 설정되면 무시
        if (curState == state) return;

        // 상태 변경 시 현재 상태 업데이트 및 해당 상태의 행동 시작
        curState = ActionState(state);
    };

    UINT GetAction() { return curState; };
protected:



private:
    // Object의 3D 모델 Transform
    Transform* meshTransform;

    // 나무의 추적 대상(Transform)
    Transform* target = nullptr;

    // 몬스터의 현재 행동 상태
    ActionState curState = IDLE;
};