#pragma once

// TopViewMonster Ŭ������ CapsuleCollider�� ��ӹ޾� ������ �⺻ ������ �����մϴ�.
class TopViewMonster : public CapsuleCollider
{
private:
    // ������ ���¸� ǥ���ϴ� ������
    enum ActionState
    {
        PATROL, // ���� ����
        TRACE,  // ���� ����
        ATTACK, // ���� ����
        NONE    // ���� ����
    };

public:
    // ������ - ������ �ʱ� ������ �����ϸ� Transform�� ���� �޽��ϴ�.
    TopViewMonster(Transform* transform);

    // �Ҹ��� - ���� ���� �ڿ��� �����մϴ�.
    ~TopViewMonster();

    // ������ ������Ʈ�� ó���ϴ� �Լ�
    void Update();

    // ���͸� �������ϴ� �Լ�
    void Render();

    // ������ ������ GUI�� �������ϴ� �Լ�
    void GUIRender();

private:
    // ������ ���� �ൿ ���¸� �����ϴ� �Լ�
    void CheckAction();

    // ������ �ൿ ���¸� �����ϴ� �Լ�
    void SetAction(ActionState state);

    // ������ ���� ������ ó���ϴ� �Լ�
    void Patrol();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Trace();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Attack();

    // ������ �پ��� �ൿ�� �����ϴ� �Լ�
    void CreateActions();

private:
    // ������ 3D �� Transform
    Transform* meshTransform;

    // ������ ���� ���(Transform)
    Transform* target = nullptr;

    // ������ �̵� �ӵ�
    float moveSpeed = 5.0f;

    // ������ ȸ�� �ӵ�
    float rotSpeed = 10.0f;

    // ������ ���� �ӵ� ����
    Vector3 velocity;

    // ������ ���� �ൿ ����
    ActionState curState = NONE;

    // ������ �پ��� �ൿ�� ��� ����
    vector<MonsterAction*> actions;
};