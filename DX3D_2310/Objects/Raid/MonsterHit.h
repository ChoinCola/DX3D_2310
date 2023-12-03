#pragma once

// MonsterTrace Ŭ������ MonsterAction�� ��ӹ޾� ������ ���� �ൿ�� �����մϴ�.
class MonsterHit : public MonsterAction
{
    const UINT MAXHITDELAY = 1.5;
public:
    // ������ - TopViewMonster �����͸� ���޹޾� �ʱ�ȭ
    MonsterHit(TopViewMonster* monster);

    // MonsterAction�� ���� ��ӵ� ���� �ൿ ������Ʈ �Լ�
    void Update() override;

    // ���� �ൿ�� ���۵� �� ȣ��Ǵ� �Լ�
    void Start() override;

private:
    float HitTime = 0;
};