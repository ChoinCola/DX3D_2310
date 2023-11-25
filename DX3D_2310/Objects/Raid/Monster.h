#pragma once

class Monster : public SphereCollider
{
private:
    enum ActionState
    {
        IDLE,

        WALK,

        HIT,
        HIT2,
        HIT3,
        HIT4,

        ATTACK,
        ATTACK2
    };

    const float EPSILON = 0.1f;

public:
    Monster();
    ~Monster();

    void Update();
    void Render();
    void GUIRender();

    void SetPlayerCollider(Collider* pc) { Playercol = pc; };

private:
    void Attack();
    void EndAttack();

    void Control();
    void Move();
    void SetAction();

    void SetState(ActionState state);

    void ReadClips();
    void SearchPlayer();

private:
    ModelAnimator* bodyMesh;
    SphereCollider* Searchcollision;

    ActionState curState;

    float moveSpeed = 10.0f;
    float rotSpeed = 2.0f;
    float accelation = 2.0f;
    float deceleration = 10.0f;

    bool AttackPos = false;

    int sword_transform = 26;
    Vector3 veloctiy;

    Vector3 prevMousePos;

    Collider* Playercol = nullptr;
    Vector3 PlayerLastPos = {};
    Vector3 PtoMdir = {};
    float SearchTime = 2.0f;
    float SearchTimeNow = 0;
};