#pragma once

// MonsterManager Ŭ������ Singleton ������ ������ ���� �� ���͸� �����ϴ� ������ �����մϴ�.
class MonsterManager : public Singleton<MonsterManager>
{
private:
    // ���� Ǯ�� ũ�⸦ ��Ÿ���� ���
    const UINT POOL_SIZE = 100;

    // ���� ���� ������ ��Ÿ���� ��� (�� ����)
    const float SPAWN_INTERVAL = 1.0f;

    // ���� ���� ��ġ�� ������ ��Ÿ���� ���
    const float SPAWN_RANGE = 20.0f;

private:
    // Singleton Ŭ������ ���� friend ����
    friend class Singleton;

    // ������ - �ܺο��� �ν��Ͻ�ȭ�� �����ϸ� �ʿ��� �ʱ�ȭ�� �����մϴ�.
    MonsterManager();

    // �Ҹ��� - �޸� ������ �����ϱ� ���� ���� ������ ���� �ڿ��� �����մϴ�.
    ~MonsterManager();

public:
    // ���� ������Ʈ �� ȣ��Ǵ� �Լ�
    void Update();

    // ���� �������� ó���ϴ� �Լ�
    void Render();

    // ������ HPBar�� ����ó���ϴ� �Լ�.
    void PostRender();
    // GUI���� ���� ������ �������ϴ� �Լ�
    void GUIRender();

    void Play(UINT index, UINT clip);

    Transform* GetClosestMonster(Vector3 pos);

    // ���� �Ŵ����� ���(Transform)�� �����ϴ� �Լ�
    void SetTarget(Transform* target) { this->target = target; }
    // ���� �Ŵ����� ���(Transform)�� ��ȯ�ϴ� �Լ�
    Transform* GetTarget() { return target; }
    ModelAnimatorInstancing* GetInstancing() { return modelInstancing; }

private:
    // ���� ������ ó���ϴ� ���� �Լ�
    void Spawn();

    void Collision();

private:
    // ���� �ν��Ͻ̿� ���Ǵ� 3D �� �ִϸ�����
    ModelAnimatorInstancing* modelInstancing;

    // ���� ��ü�� �����ϴ� ���� �����̳�
    vector<TopViewMonster*> monsters;

    // ������ ���(Transform)
    Transform* target;

    // ���� ���� ������ �����ϱ� ���� ����
    float spawnTime = 0.0f;
};