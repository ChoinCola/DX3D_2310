#pragma once

// TopViewMonster Ŭ������ Object�� ��ӹ޾� Object �⺻ ������ �����մϴ�.
class Tree : public Object
{
private:
    // ������ ���¸� ǥ���ϴ� ������
    enum ActionState
    {
        IDLE, // �⺻ ����
        SHACK,  // ��鸲 ����
    };

public:
    // ������ - ������ �ʱ� ������ �����ϸ� Transform�� ���� �޽��ϴ�.
    Tree(Transform* transform);

    // �Ҹ��� - ���� ���� �ڿ��� �����մϴ�.
    ~Tree();

    // ������ ������Ʈ�� ó���ϴ� �Լ�
    void Update();

    // ������ �������ϴ� �Լ�
    void Render();

    // ������ ������ GUI�� �������ϴ� �Լ�
    void GUIRender();

private:
    // ������ �ൿ ���¸� �����ϴ� �Լ�
    void SetAction(ActionState state);

    // ������ ���� ������ ó���ϴ� �Լ�
    void idle();

    // ������ ���� ������ ó���ϴ� �Լ�
    void Shake();

    // ������ �پ��� �ൿ�� �����ϴ� �Լ�
    void CreateActions();

private:
    // Object�� 3D �� Transform
    Transform* meshTransform;

    // ������ ���� ���(Transform)
    Transform* target = nullptr;

    // ������ ���� �ൿ ����
    ActionState curState = IDLE;
};