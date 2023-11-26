#pragma once

class Traveler : public SphereCollider
{
private:
    enum ActionState
    {
        IDLE,
        RUN_F, RUN_B, RUN_R, RUN_L,
        ATTACK
    };

    const float EPSILON = 0.1f;

public:
    Traveler();
    ~Traveler();

    void Update();
    void Render();
    void GUIRender();
    void PostRender();
    void SetMonster();


private:
    void Attack();
    void EndAttack();

    void Control();
    void Move();
    void SetAction();

    void ActiveSowdCollider();
    void InactiveSowdCollider();

    void SetState(ActionState state);

    void ReadClips();

private:
    ModelAnimator* bodyMesh;
    Sword* sword;

    Transform* rightHand;

    ActionState curState;

    float moveSpeed = 10.0f;
    float rotSpeed = 2.0f;
    float accelation = 2.0f;
    float deceleration = 10.0f;

    int sword_transform = 26;
    Vector3 veloctiy;

    Vector3 prevMousePos;

    Quad* HPbar;
    Vector3 HPBar_DefaltPos = {};
    Vector3 HPBar_DefaltScale = {1, 1, 1};

    Event attack;
private:
    float HP = 100;
    float Damage = 10;
};