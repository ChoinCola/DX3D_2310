#pragma once

// MonsterTrace Ŭ������ MonsterAction�� ��ӹ޾� ������ ���� �ൿ�� �����մϴ�.
class MonsterAttack : public MonsterAction
{
public:
    // ������ - TopViewMonster �����͸� ���޹޾� �ʱ�ȭ
    MonsterAttack(TopViewMonster* monster);

    // MonsterAction�� ���� ��ӵ� ���� �ൿ ������Ʈ �Լ�
    void Update() override;

    // ���� �ൿ�� ���۵� �� ȣ��Ǵ� �Լ�
    void Start() override;
    void SendDamage();
private:
    // ���� ������ ȸ�� �ӵ�
    float rotSpeed = 10.0f;
    float attackPower = 10.0f;
    // ���� ���(Transform)
    Transform* target = nullptr;
};