#pragma once

class TopViewMonster;

// MonsterAction Ŭ������ ������ �پ��� �ൿ�� �����ϴ� �߻� Ŭ�����Դϴ�.
class MonsterAction
{
public:
    // ������ - TopViewMonster �����͸� ���޹޾� �ʱ�ȭ
    MonsterAction(TopViewMonster* monster)
        : monster(monster) {};

    // ���� ���� �Լ��ν� ������ �ൿ ������Ʈ�� ���
    virtual void Update() = 0;

    // �ൿ�� ���۵� �� ȣ��Ǵ� ���� �Լ�
    virtual void Start() {};

protected:
    // ���͸� �־��� �������� �̵���Ű�� �Լ�
    void Move(Vector3 direction, float MoveSpeed, float rotSpeed);

    // ���Ͱ� �־��� �������� ȸ���ϵ��� �ϴ� �Լ�
    void LookAtRotate(Vector3 direction, float rotSpeed);

protected:
    // ���� ��ü�� ���� ������
    TopViewMonster* monster;
};