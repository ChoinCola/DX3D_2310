#pragma once

// TopViewMonster Ŭ������ CapsuleCollider�� ��ӹ޾� ������ �⺻ ������ �����մϴ�.
class TopViewMonster : public CapsuleCollider
{

private:
    const float DELETETIME = 5;
    const float ATTACKDELAY = 1;
    // ������ ���¸� ǥ���ϴ� ������
public:
    enum ActionState
    {
        PATROL, // ���� ����
        TRACE,  // ���� ����
        ATTACK, // ���� ����
        HIT,    // �´� ����
        DEAD,   // �״� ����
        NONE    // ���� ����
    };

public:
    // ������ - ������ �ʱ� ������ �����ϸ� Transform�� ���� �޽��ϴ�.
    TopViewMonster(Transform* transform, UINT num);

    // �Ҹ��� - ���� ���� �ڿ��� �����մϴ�.
    ~TopViewMonster();

    // ������ ������Ʈ�� ó���ϴ� �Լ�
    void Update();

    // ���͸� �������ϴ� �Լ�
    void Render();

    // ������ ������ GUI�� �������ϴ� �Լ�
    void GUIRender();

    void Hit(float input);

    // ������ �ൿ ���¸� �����ϴ� �Լ�
    void SetAction(ActionState state);

    ActionState GetAction() { return curState; }

    UINT Getnumber() { return monsternumber; }
    bool NowChangeMotion() { return ChangeMotion; }
private:
    // ������ ���� �ൿ ���¸� �����ϴ� �Լ�
    void CheckAction();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Patrol();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Trace();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Attack();

    // ������ �پ��� �ൿ�� �����ϴ� �Լ�
    void CreateActions();

    void DeadObejctDelete();

private:
    // ������ 3D �� Transform
    Transform* meshTransform;

    // ������ ���� ���(Transform)
    Transform* target = nullptr;

    // ������ ��ȣ.
    UINT monsternumber = 0;
    // ������ �̵� �ӵ�
    float moveSpeed = 5.0f;

    // ������ ȸ�� �ӵ�
    float rotSpeed = 10.0f;

    float HP = 100;
    float DeadTime = 0;
    float Attime = 0;
    bool ChangeMotion = false;
    // ������ ���� �ӵ� ����
    Vector3 velocity;

    // ������ ���� �ൿ ����
    ActionState curState = NONE;

    // ������ �پ��� �ൿ�� ��� ����
    vector<MonsterAction*> actions;
};