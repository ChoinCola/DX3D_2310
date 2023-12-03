#pragma once

// MonsterManager Ŭ������ Singleton ������ ������ ���� �� ���͸� �����ϴ� ������ �����մϴ�.
class ObjectMaster : public Singleton<ObjectMaster>
{

    const UINT Treecount = 1;
    const UINT Grasscount = 30;
private:
    // Object Ǯ�� ũ�⸦ ��Ÿ���� ���
    const UINT POOL_SIZE = 1800;
    // Object ���� ��ġ�� ������ ��Ÿ���� ���
    const float SPAWN_RANGE = 20.0f;

private:
    // Singleton Ŭ������ ���� friend ����
    friend class Singleton;

    // ������ - �ܺο��� �ν��Ͻ�ȭ�� �����ϸ� �ʿ��� �ʱ�ȭ�� �����մϴ�.
    ObjectMaster();

    // �Ҹ��� - �޸� ������ �����ϱ� ���� Object ������ ���� �ڿ��� �����մϴ�.
    ~ObjectMaster();

public:
    // ���� ������Ʈ �� ȣ��Ǵ� �Լ�
    void Update();

    // Object �������� ó���ϴ� �Լ�
    void Render();

    void Rendermap(pair<float, float> inputpos, Float2 size);
    void Collisionmap(pair<float, float> inputpos, Collider* input);
    void AnimUpdatToemap(pair<float, float> inputpos);
    void OutputmapToObject(pair<float, float> input);
    // GUI���� ���� ������ �������ϴ� �Լ�
    void GUIRender();

    // Object �Ŵ����� ���(Transform)�� �����ϴ� �Լ�
    void SetTarget(Transform* target) { this->target = target; }
    void SetTerrain(vector<Terrain*>* terrain);

    // Object �Ŵ����� ���(Transform)�� ��ȯ�ϴ� �Լ�
    Transform* GetTarget() { return target; }

    void ColliderRender(pair<float, float> inputpos);
    void GUIRendermap(pair<float, float> inputpos);

private:
    // Object ������ ó���ϴ� ���� �Լ�
    void Spawn();

    void MakeTree(pair<float, float> input, Float2 size);
    void MakeGrass(pair<float, float> input, Float2 size);
    
    void inputTransform(Object* input, ModelAnimatorInstancing* output);

    void RemakeTransform(vector<Object*> input, vector<Transform*>& output);

private:
    // Object �ν��Ͻ̿� ���Ǵ� 3D �� �ִϸ�����
    ModelAnimatorInstancing* modelInstancing;

    // Ojbect ��ü�� �����ϴ� ���� �����̳�
    vector<Object*> monsters;

    map<pair<float, float>, vector<Object*>> TreePos;
    ModelAnimatorInstancing* Trees;

    map<pair<float, float>, vector<Object*>> GrassPos;
    ModelAnimatorInstancing* Grasses;

    // Object ���(Transform)
    Transform* target;

    int clip = 0;
    int instanceIndex = 0;
};