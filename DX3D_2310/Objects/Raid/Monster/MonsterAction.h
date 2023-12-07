#pragma once

class TopViewMonster;

// MonsterAction 클래스는 몬스터의 다양한 행동을 정의하는 추상 클래스입니다.
class MonsterAction
{
public:
    // 생성자 - TopViewMonster 포인터를 전달받아 초기화
    MonsterAction(TopViewMonster* monster)
        : monster(monster) {};

    // 순수 가상 함수로써 몬스터의 행동 업데이트를 담당
    virtual void Update() = 0;

    // 행동이 시작될 때 호출되는 가상 함수
    virtual void Start() {};

protected:
    // 몬스터를 주어진 방향으로 이동시키는 함수
    void Move(Vector3 direction, float MoveSpeed, float rotSpeed);

    // 몬스터가 주어진 방향으로 회전하도록 하는 함수
    void LookAtRotate(Vector3 direction, float rotSpeed);

protected:
    // 몬스터 객체에 대한 포인터
    TopViewMonster* monster;
};