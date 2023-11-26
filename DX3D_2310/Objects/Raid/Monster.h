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
        ATTACK2,
        
        DEATH
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

    void Hitnow(void* collider);

    void Control();
    void Move();
    void SetAction();

    void SetState(ActionState state);

    void ReadClips();
    void SearchPlayer();
    void Death();

private:
    ModelAnimator* bodyMesh;
    SphereCollider* Searchcollision;

    ActionState curState;

    float moveSpeed = 10.0f;
    float rotSpeed = 2.0f;
    float accelation = 2.0f;
    float deceleration = 10.0f;

    bool AttackPos = false;

    Vector3 veloctiy;

    Vector3 prevMousePos;

    Collider* Playercol = nullptr;
    Vector3 PlayerLastPos = {};
    Vector3 PtoMdir = {};

    list<Monster*> immolation;
    float SearchTime = 2.0f;
    float SearchTimeNow = 0;

private:
    float HP = 100.0f;
    float damage = 10.0f;

    bool Hitself = false;
    bool death = false;
    HPbar* hpbar = nullptr;
};