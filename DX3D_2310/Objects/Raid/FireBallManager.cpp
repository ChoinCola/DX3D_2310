#include "Framework.h"
// FireBallManager 클래스의 생성자
FireBallManager::FireBallManager()
{
    // ModelInstancing을 생성하고 이름을 설정합니다.
    modelInstancing = new ModelInstancing("FireBall");

    // 첫 번째 메시에 대한 블렌드 스테이트를 추가적으로 설정합니다.
    modelInstancing->GetMesh(0)->GetBlendState()->Additive();
    // 첫 번째 메시에 대한 래스터라이저 스테이트를 양면 시점으로 설정합니다.
    modelInstancing->GetMesh(0)->GetRasterizerState()->CullMode(D3D11_CULL_NONE);

    // FireBall 객체들을 모두 할당하고 초기화합니다.
    fireBalls.resize(FULL_SIZE);
    for (FireBall*& fireBall : fireBalls)
    {
        fireBall = new FireBall(modelInstancing->Add());
    }
}

// FireBallManager 클래스의 소멸자
FireBallManager::~FireBallManager()
{
    // ModelInstancing을 메모리에서 해제합니다.
    delete modelInstancing;

    // FireBall 객체들을 순회하면서 메모리에서 해제합니다.
    for (FireBall* fireBall : fireBalls)
        delete fireBall;
}

// FireBallManager 클래스의 업데이트 함수
void FireBallManager::Update()
{
    // 각 FireBall 객체를 업데이트합니다.
    for (FireBall* fireBall : fireBalls)
        fireBall->Update();

    // ModelInstancing을 업데이트합니다.
    modelInstancing->Update();
}

// FireBallManager 클래스의 렌더링 함수
void FireBallManager::Render()
{
    // ModelInstancing을 통해 인스턴싱된 객체들을 렌더링합니다.
    modelInstancing->Render();

    // 각 FireBall 객체를 렌더링합니다.
    for (FireBall* fireBall : fireBalls)
        fireBall->Render();
}

// FireBallManager 클래스의 GUI 렌더링 함수
void FireBallManager::GUIRender()
{
    // ModelInstancing을 통해 인스턴싱된 객체들의 GUI를 렌더링합니다.
    modelInstancing->GUIRender();

    // 각 FireBall 객체의 GUI를 렌더링합니다.
    for (FireBall* fireBall : fireBalls)
        fireBall->GUIRender();
}

// 새로운 FireBall을 발사하는 함수
void FireBallManager::Fire(Vector3 pos, Vector3 direction)
{
    // 비활성화된 FireBall을 찾아 발사합니다.
    for (FireBall* fireBall : fireBalls)
    {
        if (!fireBall->IsActive())
        {
            fireBall->Fire(pos, direction);
            return;
        }
    }
}