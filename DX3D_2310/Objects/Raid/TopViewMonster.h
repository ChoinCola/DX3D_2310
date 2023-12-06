#pragma once

// TopViewMonster Ŭ������ CapsuleCollider�� ��ӹ޾� ������ �⺻ ������ �����մϴ�.
class TopViewMonster : public CapsuleCollider
{
public:

    enum AnimState
    {
        WALK, RUN, ATK, HIT, DEAD
    };

    enum ActionState
    {
        PATROL, // ���� ����
        TRACE,  // ���� ����
        ATTACK, // ���� ����
        DAMAGE,    // �´� ����
        DIE,   // �״� ����
        NONE    // ���� ����
    };

private:

    const float TRACE_RANGE = 10.0f;
    const float ATTACK_RANGE = 2.0f;

    const float DELETETIME = 5;
    const float ATTACKDELAY = 1;
    // ������ ���¸� ǥ���ϴ� ������

public:
    // ������ - ������ �ʱ� ������ �����ϸ� Transform�� ���� �޽��ϴ�.
    TopViewMonster(Transform* transform, ModelAnimatorInstancing* instancing, UINT num);

    // �Ҹ��� - ���� ���� �ڿ��� �����մϴ�.
    ~TopViewMonster();

    // ������ ������Ʈ�� ó���ϴ� �Լ�
    void Update();

    // ���͸� �������ϴ� �Լ�
    void Render();
    void PostRender();
    // ������ ������ GUI�� �������ϴ� �Լ�
    void GUIRender();

    void Hit(float input);

    // ������ �ൿ ���¸� �����ϴ� �Լ�
    void Play(AnimState clip);
    void SetAction(ActionState state);

    void SetEvent(int clip, Event event, float timeRatio);
    void ExcuteEvent();

    ActionState GetAction() { return curState; }

    UINT Getnumber() { return index; }
    bool NowChangeMotion() { return ChangeMotion; }


private:
    // ������ ���� �ൿ ���¸� �����ϴ� �Լ�
    void CheckAction();

    void SetHPBar();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Patrol();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Trace();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Attack();

    // ������ �پ��� �ൿ�� �����ϴ� �Լ�
    void CreateActions();

    void EndDamage();
    void EndAttack();

    void DeadObejctDelete();

private:
    // ������ 3D �� Transform
    Transform* meshTransform;

    // ������ ���� ���(Transform)
    Transform* target = nullptr;

    // ������ ��ȣ.
    UINT index = 0;
    // ������ �̵� �ӵ�
    float moveSpeed = 5.0f;

    // ������ ȸ�� �ӵ�
    float rotSpeed = 10.0f;

    float maxHP = 100.0f;
    float curHP = 100.0f;
    float DeadTime = 0;
    float Attime = 0;
    bool ChangeMotion = false;
    // ������ ���� �ӵ� ����
    Vector3 velocity;

    // ������ ���� �ൿ ����
    ActionState curState = NONE;
    // ������ ���� �ִϸ��̼� ����
    AnimState animState = RUN;

    ModelAnimatorInstancing* instancing;
    ModelAnimatorInstancing::Motion* motion;
    
    // ������ �پ��� �ൿ�� ��� ����
    vector<MonsterAction*> actions;

    vector<map<float, Event>> totalEvent;
    vector<map<float, Event>::iterator> eventIters;

    ProgressBar* hpBar;
    Vector3 hpBarOffset;
};