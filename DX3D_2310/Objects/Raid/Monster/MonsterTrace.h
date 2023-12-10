#pragma once

// MonsterTrace 클래스는 MonsterAction을 상속받아 몬스터의 추적 행동을 정의합니다.
class MonsterTrace : public MonsterAction
{
public:
    // 생성자 - TopViewMonster 포인터를 전달받아 초기화
    MonsterTrace(TopViewMonster* monster);

    // MonsterAction을 통해 상속된 추적 행동 업데이트 함수
    void Update() override;

    // 추적 행동이 시작될 때 호출되는 함수
    void Start() override;

private:
    // 추적 동작의 이동 속도
    float moveSpeed;

    // 추적 대상(Transform)
    Transform* target = nullptr;
};