#pragma once
class Object : public SphereCollider
{
private:
    // ������Ʈ ���¸� ǥ���ϴ� ������
    enum ActionState
    {
        IDLE, // �⺻ ����
        SHACK,  // ��鸲 ����
    };

public:
    // ������ - ������ �ʱ� ������ �����ϸ� Transform�� ���� �޽��ϴ�.
    Object(Transform* transform) : meshTransform(transform) 
    {
        this->SetLocalPosition(transform->GetLocalPosition());
        this->SetLocalRotation(transform->GetLocalRotation());
        this->SetLocalScale(transform->GetLocalScale());

        this->SetTag("Object");
        this->Load();
        // ���ʹ� ó������ ��Ȱ��ȭ ���·� ����
        isActive = false;

        // �ʱ� �ൿ ���¸� Idle�� ����
        SetAction(IDLE);
    };

    // �Ҹ��� - ���� ���� �ڿ��� �����մϴ�.
    ~Object() = default;

    // ������ ������Ʈ�� ó���ϴ� �Լ�
    void Update() {     // ���Ͱ� ��Ȱ��ȭ ������ ��� ������Ʈ�� �������� ����
        if (!IsActive()) return;

        // �θ� Ŭ������ UpdateWorld �Լ� ȣ��
        UpdateWorld();
    };

    // ������ �������ϴ� �Լ�
    void Render() {
        // ���Ͱ� ��Ȱ��ȭ ������ ��� �������� �������� ����
        if (!IsActive()) return;

        // CapsuleCollider�� Render �Լ� ȣ��
        __super::Render();
    };

    // ������ ������ GUI�� �������ϴ� �Լ�
    void GUIRender() {

        // ���Ͱ� ��Ȱ��ȭ ������ ��� GUI �������� �������� ����
        if (!IsActive()) return;

        // CapsuleCollider�� GUIRender �Լ� ȣ��
        __super::GUIRender();
    };


    UINT GetState() { return curState; }

    // ������ �ൿ ���¸� �����ϴ� �Լ�
    void SetAction(UINT state)
    {
        // ���� ���¿� ������ ���·� �����Ǹ� ����
        if (curState == state) return;

        // ���� ���� �� ���� ���� ������Ʈ �� �ش� ������ �ൿ ����
        curState = ActionState(state);
    };

    UINT GetAction() { return curState; };

    void EndAnim() { SetAction(IDLE); };

    void SetObejctnum(int input) { Objectnum = input; }
    UINT GetObejctnum() { return Objectnum; }
protected:



private:
    // Object�� 3D �� Transform
    Transform* meshTransform;

    // ������ ���� ���(Transform)
    Transform* target = nullptr;

    // ������ ���� �ൿ ����
    ActionState curState = IDLE;

    int Objectnum = 0;
};