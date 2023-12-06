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
    FOR(POOL_SIZE)
    {
        monsters[i] = new TopViewMonster(modelInstancing->Add(), modelInstancing, i);
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
    Collision();
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

void MonsterManager::PostRender()
{
    for (TopViewMonster* monster : monsters)
        monster->PostRender();
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

void MonsterManager::Play(UINT index, UINT clip)
{
    modelInstancing->PlayClip(index, clip);
}

Transform* MonsterManager::GetClosestMonster(Vector3 pos)
{
    float minDistance = FLT_MAX;
    Transform* closestMosnter = nullptr;

    for (TopViewMonster* monster : monsters)
    {
        if (!monster->IsActive()) continue;

        float distance = MATH->Distance(pos, monster->GetLocalPosition());

        if (minDistance > distance)
        {
            closestMosnter = monster;
            minDistance = distance;
        }
    }
    return closestMosnter;
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

void MonsterManager::Collision()
{
    for (TopViewMonster* monster : monsters)
    {
        if (FireBallManager::Get()->IsCollision(monster))
        {
            monster->SetAction(TopViewMonster::DAMAGE);
        }
    }
}