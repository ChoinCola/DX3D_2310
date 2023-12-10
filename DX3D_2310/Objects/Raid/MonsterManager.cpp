#include "Framework.h"

// MonsterManager 생성자
MonsterManager::MonsterManager()
{
    // 몬스터에 사용될 3D 모델 애니메이터 초기화
    modelInstancing = new ModelAnimatorInstancing("Monster");
    modelInstancing->ReadClip("Walk");
    modelInstancing->ReadClip("Run");
    modelInstancing->ReadClip("Attack1");
    modelInstancing->ReadClip("Hit1");
    modelInstancing->ReadClip("death");
    modelInstancing->CreateTexture();

    // 몬스터 배열 크기 설정
    monsters.resize(POOL_SIZE);

    // 몬스터 객체 초기화 및 모델 인스턴스에 추가
    FOR(POOL_SIZE)
    {
        monsters[i] = new TopViewMonster(modelInstancing->Add(), modelInstancing, i);
    }
}

// MonsterManager 소멸자
MonsterManager::~MonsterManager()
{
    delete modelInstancing;

    // 할당된 몬스터 객체들을 메모리에서 해제
    for (TopViewMonster* monster : monsters)
        delete monster;
}

// 게임 업데이트 시 호출되는 함수
void MonsterManager::Update()
{
    // 몬스터 생성 간격 업데이트
    spawnTime += DELTA;

    // 일정 간격마다 몬스터 생성 함수 호출
    if (spawnTime >= SPAWN_INTERVAL)
    {
        spawnTime -= SPAWN_INTERVAL;
        Spawn();
    }

    Collision();

    // 모델 인스턴싱 업데이트
    modelInstancing->Update();

    // 각 몬스터의 업데이트 함수 호출
    for (TopViewMonster* monster : monsters)
        monster->Update();
}

// 몬스터 렌더링을 처리하는 함수
void MonsterManager::Render()
{
    // 모델 인스턴싱 렌더링
    modelInstancing->Render();

    // 각 몬스터의 렌더링 함수 호출
    for (TopViewMonster* monster : monsters)
        monster->Render();
}

// GUI에서 몬스터 정보를 렌더링하는 함수
void MonsterManager::GUIRender()
{
    // 모델 인스턴싱의 GUI 렌더링
    modelInstancing->GUIRender();

    // 각 몬스터의 GUI 렌더링 함수 호출
    for (TopViewMonster* monster : monsters)
        monster->GUIRender();
}

void MonsterManager::Play(UINT index, UINT clip)
{
    modelInstancing->PlayClip(index, clip);
}

// 몬스터 생성을 처리하는 함수
void MonsterManager::Spawn()
{
    // 비활성화된 몬스터를 찾아 생성
    for (TopViewMonster* monster : monsters)
    {
        if (!monster->IsActive())
        {
            // 몬스터를 활성화하고 생성 위치 설정
            monster->SetActive(true);
            Vector3 pos = target->GetGlobalPosition();
            Vector3 direction;
            direction.x = MATH->Random(-1.0f, 1.0f);
            direction.z = MATH->Random(-1.0f, 1.0f);

            pos += direction.GetNormalized() * SPAWN_RANGE;
            monster->SetLocalPosition(pos);

            // 생성 후 바로 리턴하여 하나의 몬스터만 생성
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