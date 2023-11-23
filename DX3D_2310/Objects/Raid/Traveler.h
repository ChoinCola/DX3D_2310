#pragma once

class Traveler : public CapsuleCollider
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
};