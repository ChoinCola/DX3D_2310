#pragma once

// MonsterTrace Ŭ������ MonsterAction�� ��ӹ޾� ������ ���� �ൿ�� �����մϴ�.
class MonsterTrace : public MonsterAction
{
public:
    // ������ - TopViewMonster �����͸� ���޹޾� �ʱ�ȭ
    MonsterTrace(TopViewMonster* monster);

    // MonsterAction�� ���� ��ӵ� ���� �ൿ ������Ʈ �Լ�
    void Update() override;

    // ���� �ൿ�� ���۵� �� ȣ��Ǵ� �Լ�
    void Start() override;

private:
    // ���� ������ �̵� �ӵ�
    float moveSpeed;

    // ���� ���(Transform)
    Transform* target = nullptr;
};