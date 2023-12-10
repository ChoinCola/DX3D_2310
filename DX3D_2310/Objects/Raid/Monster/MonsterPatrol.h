#pragma once

// MonsterPatrol Ŭ������ MonsterAction�� ��ӹ޾� ������ ���� �ൿ�� �����մϴ�.
class MonsterPatrol : public MonsterAction
{
private:
    // ���� ������ ��Ÿ���� ���
    const float RANGE = 10.0f;

public:
    // ������ - TopViewMonster �����͸� ���޹޾� �ʱ�ȭ
    MonsterPatrol(TopViewMonster* monster);

    // MonsterAction�� ���� ��ӵ� ���� �ൿ ������Ʈ �Լ�
    void Update() override;

    // ���� �ൿ�� ���۵� �� ȣ��Ǵ� �Լ�
    void Start() override;

private:
    // ���� ��ǥ ��ġ�� �����ϴ� �Լ�
    void SetDestPos();

private:
    // ���� ������ �̵� �ӵ�
    float moveSpeed = 3.0f;

    // ���� ������ ȸ�� �ӵ�
    float rotSpeed = 10.0f;

    // ���� ��ǥ ��ġ
    Vector3 destPos;
};