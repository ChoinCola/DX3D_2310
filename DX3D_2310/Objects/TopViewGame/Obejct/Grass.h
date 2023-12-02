#pragma once
// TopViewMonster Ŭ������ Object�� ��ӹ޾� Object �⺻ ������ �����մϴ�.
class Grass : public Object
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
    Grass(Transform* transform);

    // �Ҹ��� - ���� ���� �ڿ��� �����մϴ�.
    ~Grass();

    // ������ ������Ʈ�� ó���ϴ� �Լ�
    void Update();

    // ������ �������ϴ� �Լ�
    void Render();

    // ������ ������ GUI�� �������ϴ� �Լ�
    void GUIRender();

    // ������ �ൿ ���¸� �����ϴ� �Լ�
    void SetAction(UINT state);
private:
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