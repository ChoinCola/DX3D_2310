#pragma once

// MonsterPatrol 클래스는 MonsterAction을 상속받아 몬스터의 순찰 행동을 정의합니다.
class MonsterPatrol : public MonsterAction
{
private:
    // 순찰 범위를 나타내는 상수
    const float RANGE = 10.0f;

public:
    // 생성자 - TopViewMonster 포인터를 전달받아 초기화
    MonsterPatrol(TopViewMonster* monster);

    // MonsterAction을 통해 상속된 순찰 행동 업데이트 함수
    void Update() override;

    // 순찰 행동이 시작될 때 호출되는 함수
    void Start() override;

private:
    // 순찰 목표 위치를 설정하는 함수
    void SetDestPos();

private:
    // 순찰 동작의 이동 속도
    float moveSpeed = 3.0f;

    // 순찰 동작의 회전 속도
    float rotSpeed = 10.0f;

    // 순찰 목표 위치
    Vector3 destPos;
};