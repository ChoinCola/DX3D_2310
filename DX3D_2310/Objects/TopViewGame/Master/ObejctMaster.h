#pragma once

// MonsterManager 클래스는 Singleton 패턴을 따르며 게임 내 몬스터를 관리하는 역할을 수행합니다.
class ObjectMaster : public Singleton<ObjectMaster>
{

    const UINT Treecount = 1;
    const UINT Grasscount = 30;
private:
    // Object 풀의 크기를 나타내는 상수
    const UINT POOL_SIZE = 1800;
    // Object 생성 위치의 범위를 나타내는 상수
    const float SPAWN_RANGE = 20.0f;

private:
    // Singleton 클래스에 대한 friend 선언
    friend class Singleton;

    // 생성자 - 외부에서 인스턴스화를 방지하며 필요한 초기화를 수행합니다.
    ObjectMaster();

    // 소멸자 - 메모리 누수를 방지하기 위해 Object 관리에 사용된 자원을 정리합니다.
    ~ObjectMaster();

public:
    // 게임 업데이트 시 호출되는 함수
    void Update();

    // Object 렌더링을 처리하는 함수
    void Render();

    void Rendermap(pair<float, float> inputpos, Float2 size);
    void Collisionmap(pair<float, float> inputpos, Collider* input);
    void AnimUpdatToemap(pair<float, float> inputpos);
    void OutputmapToObject(pair<float, float> input);
    // GUI에서 몬스터 정보를 렌더링하는 함수
    void GUIRender();

    // Object 매니저의 대상(Transform)을 설정하는 함수
    void SetTarget(Transform* target) { this->target = target; }
    void SetTerrain(vector<Terrain*>* terrain);

    // Object 매니저의 대상(Transform)을 반환하는 함수
    Transform* GetTarget() { return target; }

    void ColliderRender(pair<float, float> inputpos);
    void GUIRendermap(pair<float, float> inputpos);

private:
    // Object 생성을 처리하는 내부 함수
    void Spawn();

    void MakeTree(pair<float, float> input, Float2 size);
    void MakeGrass(pair<float, float> input, Float2 size);
    
    void inputTransform(Object* input, ModelAnimatorInstancing* output);

    void RemakeTransform(vector<Object*> input, vector<Transform*>& output);

private:
    // Object 인스턴싱에 사용되는 3D 모델 애니메이터
    ModelAnimatorInstancing* modelInstancing;

    // Ojbect 객체를 저장하는 벡터 컨테이너
    vector<Object*> monsters;

    map<pair<float, float>, vector<Object*>> TreePos;
    ModelAnimatorInstancing* Trees;

    map<pair<float, float>, vector<Object*>> GrassPos;
    ModelAnimatorInstancing* Grasses;

    // Object 대상(Transform)
    Transform* target;

    int clip = 0;
    int instanceIndex = 0;
};