#pragma once

// MonsterManager 클래스는 Singleton 패턴을 따르며 게임 내 몬스터를 관리하는 역할을 수행합니다.
class MonsterManager : public Singleton<MonsterManager>
{
private:
    // 몬스터 풀의 크기를 나타내는 상수
    const UINT POOL_SIZE = 100;

    // 몬스터 생성 간격을 나타내는 상수 (초 단위)
    const float SPAWN_INTERVAL = 1.0f;

    // 몬스터 생성 위치의 범위를 나타내는 상수
    const float SPAWN_RANGE = 20.0f;

private:
    // Singleton 클래스에 대한 friend 선언
    friend class Singleton;

    // 생성자 - 외부에서 인스턴스화를 방지하며 필요한 초기화를 수행합니다.
    MonsterManager();

    // 소멸자 - 메모리 누수를 방지하기 위해 몬스터 관리에 사용된 자원을 정리합니다.
    ~MonsterManager();

public:
    // 게임 업데이트 시 호출되는 함수
    void Update();

    // 몬스터 렌더링을 처리하는 함수
    void Render();

    // 몬스터의 HPBar를 렌더처리하는 함수.
    void PostRender();
    // GUI에서 몬스터 정보를 렌더링하는 함수
    void GUIRender();

    void Play(UINT index, UINT clip);

    Transform* GetClosestMonster(Vector3 pos);

    // 몬스터 매니저의 대상(Transform)을 설정하는 함수
    void SetTarget(Transform* target) { this->target = target; }
    // 몬스터 매니저의 대상(Transform)을 반환하는 함수
    Transform* GetTarget() { return target; }
    ModelAnimatorInstancing* GetInstancing() { return modelInstancing; }

private:
    // 몬스터 생성을 처리하는 내부 함수
    void Spawn();

    void Collision();

private:
    // 몬스터 인스턴싱에 사용되는 3D 모델 애니메이터
    ModelAnimatorInstancing* modelInstancing;

    // 몬스터 객체를 저장하는 벡터 컨테이너
    vector<TopViewMonster*> monsters;

    // 몬스터의 대상(Transform)
    Transform* target;

    // 몬스터 생성 간격을 추적하기 위한 변수
    float spawnTime = 0.0f;
};