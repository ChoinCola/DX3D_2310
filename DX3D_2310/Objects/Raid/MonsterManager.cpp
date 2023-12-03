#include "Framework.h"

// MonsterManager ������
MonsterManager::MonsterManager()
{
    // ���Ϳ� ���� 3D �� �ִϸ����� �ʱ�ȭ
    modelInstancing = new ModelAnimatorInstancing("Monster");
    modelInstancing->ReadClip("Walk");
    modelInstancing->ReadClip("Run");
    modelInstancing->ReadClip("Attack1");
    modelInstancing->ReadClip("Hit1");
    modelInstancing->ReadClip("death");
    modelInstancing->CreateTexture();

    // ���� �迭 ũ�� ����
    monsters.resize(POOL_SIZE);

    // ���� ��ü �ʱ�ȭ �� �� �ν��Ͻ��� �߰�
    FOR(monsters.size())
    {
        monsters[i] = new TopViewMonster(modelInstancing->Add(), i);
    }
}

// MonsterManager �Ҹ���
MonsterManager::~MonsterManager()
{
    // �Ҵ�� ���� ��ü���� �޸𸮿��� ����
    for (TopViewMonster* monster : monsters)
        delete monster;
}

// ���� ������Ʈ �� ȣ��Ǵ� �Լ�
void MonsterManager::Update()
{
    // ���� ���� ���� ������Ʈ
    spawnTime += DELTA;

    // ���� ���ݸ��� ���� ���� �Լ� ȣ��
    if (spawnTime >= SPAWN_INTERVAL)
    {
        spawnTime -= SPAWN_INTERVAL;
        Spawn();
    }

    // �� �ν��Ͻ� ������Ʈ
    modelInstancing->Update();

    // �� ������ ������Ʈ �Լ� ȣ��
    for (TopViewMonster* monster : monsters)
        monster->Update();

    ChackAnim();
}

// ���� �������� ó���ϴ� �Լ�
void MonsterManager::Render()
{
    // �� �ν��Ͻ� ������
    modelInstancing->Render();

    // �� ������ ������ �Լ� ȣ��
    for (TopViewMonster* monster : monsters)
        monster->Render();
}

// GUI���� ���� ������ �������ϴ� �Լ�
void MonsterManager::GUIRender()
{
    // �� �ν��Ͻ��� GUI ������
    modelInstancing->GUIRender();

    // �� ������ GUI ������ �Լ� ȣ��
    for (TopViewMonster* monster : monsters)
        monster->GUIRender();
}

// ���� ������ ó���ϴ� �Լ�
void MonsterManager::Spawn()
{
    // ��Ȱ��ȭ�� ���͸� ã�� ����
    for (TopViewMonster* monster : monsters)
    {
        if (!monster->IsActive())
        {
            // ���͸� Ȱ��ȭ�ϰ� ���� ��ġ ����
            monster->SetActive(true);
            Vector3 pos = target->GetGlobalPosition();
            Vector3 direction;
            direction.x = MATH->Random(-1.0f, 1.0f);
            direction.z = MATH->Random(-1.0f, 1.0f);

            pos += direction.GetNormalized() * SPAWN_RANGE;
            monster->SetLocalPosition(pos);

            // ���� �� �ٷ� �����Ͽ� �ϳ��� ���͸� ����
            return;
        }
    }
}

void MonsterManager::ChackAnim()
{
    for (TopViewMonster* monster : monsters)
    {
        if (!monster->NowChangeMotion())
            continue;

        switch (monster->GetAction())
        {
        case TopViewMonster::PATROL:
            modelInstancing->PlayClip(monster->Getnumber(), 0);
            break;
        case TopViewMonster::TRACE:
            modelInstancing->PlayClip(monster->Getnumber(), 1);
            break;
        case TopViewMonster::ATTACK:
            modelInstancing->PlayClip(monster->Getnumber(), 2);
            break;
        case TopViewMonster::HIT:
            modelInstancing->PlayClip(monster->Getnumber(), 3);
            break;
        case TopViewMonster::DEAD:
            modelInstancing->PlayClip(monster->Getnumber(), 4);
            break;
        case TopViewMonster::NONE:
            break;
        }
    }
}
