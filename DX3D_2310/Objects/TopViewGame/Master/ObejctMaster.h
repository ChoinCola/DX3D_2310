#pragma once

// MonsterManager Ŭ������ Singleton ������ ������ ���� �� ���͸� �����ϴ� ������ �����մϴ�.
class ObjectMaster : public Singleton<ObjectMaster>
{
private:
    // Object Ǯ�� ũ�⸦ ��Ÿ���� ���
    const UINT POOL_SIZE = 100;
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

    // GUI���� ���� ������ �������ϴ� �Լ�
    void GUIRender();

    // Object �Ŵ����� ���(Transform)�� �����ϴ� �Լ�
    void SetTarget(Transform* target) { this->target = target; }
    void SetTerrain(vector<Terrain*>* terrain);

    // Object �Ŵ����� ���(Transform)�� ��ȯ�ϴ� �Լ�
    Transform* GetTarget() { return target; }

private:
    // Object ������ ó���ϴ� ���� �Լ�
    void Spawn();

private:
    // Object �ν��Ͻ̿� ���Ǵ� 3D �� �ִϸ�����
    ModelAnimatorInstancing* modelInstancing;

    // Ojbect ��ü�� �����ϴ� ���� �����̳�
    vector<Object*> monsters;

    // Object ���(Transform)
    Transform* target;
};