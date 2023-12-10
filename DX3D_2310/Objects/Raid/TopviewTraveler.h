#pragma once

class TopviewTraveler : public CapsuleCollider
{
private:
    enum ActionState
    {
        IDLE,
        WALK,
        ATTACK,
    };

    const float EPSILON = 0.1f;

public:
    TopviewTraveler();
    ~TopviewTraveler();

    void Update();
    void Render();
    void GUIRender();
    void PostRender();
    void Hit(float damage);
private:
    void Attack();
    void EndAttack();

    void Control();
    void Move();
    void SetAction();

    void SetState(ActionState state);

    void ReadClips();
    void CreateWeapons();
    void SearchPlayer();

    void Death();

private:
    ModelAnimator* bodyMesh;

    Transform* rightHand;

    ActionState curState = IDLE;

    float moveSpeed = 10.0f;
    float rotSpeed = 10.0f;

    float curHP = 100;
    Vector3 velocity;

};