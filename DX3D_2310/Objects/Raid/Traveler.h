#pragma once

class Traveler : public CapsuleCollider
{
private:
    enum ActionState
    {
        IDLE,
        ATTACK
    };

public:
    Traveler();
    ~Traveler();

    void Update();
    void Render();
    void GUIRender();

private:
    void Attack();
    void EndAttack();

    void SetState(ActionState state);

    void ReadClips();

private:
    ModelAnimator* bodyMesh;
    Model* sword;

    Transform* rightHand;

    ActionState curState;
};