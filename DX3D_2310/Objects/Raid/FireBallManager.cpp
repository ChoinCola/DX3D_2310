#include "Framework.h"
// FireBallManager Ŭ������ ������
FireBallManager::FireBallManager()
{
    // ModelInstancing�� �����ϰ� �̸��� �����մϴ�.
    modelInstancing = new ModelInstancing("FireBall");

    // ù ��° �޽ÿ� ���� ���� ������Ʈ�� �߰������� �����մϴ�.
    modelInstancing->GetMesh(0)->GetBlendState()->Additive();
    // ù ��° �޽ÿ� ���� �����Ͷ����� ������Ʈ�� ��� �������� �����մϴ�.
    modelInstancing->GetMesh(0)->GetRasterizerState()->CullMode(D3D11_CULL_NONE);

    // FireBall ��ü���� ��� �Ҵ��ϰ� �ʱ�ȭ�մϴ�.
    fireBalls.resize(FULL_SIZE);
    for (FireBall*& fireBall : fireBalls)
    {
        fireBall = new FireBall(modelInstancing->Add());
    }
}

// FireBallManager Ŭ������ �Ҹ���
FireBallManager::~FireBallManager()
{
    // ModelInstancing�� �޸𸮿��� �����մϴ�.
    delete modelInstancing;

    // FireBall ��ü���� ��ȸ�ϸ鼭 �޸𸮿��� �����մϴ�.
    for (FireBall* fireBall : fireBalls)
        SAFE_DELETE(fireBall);
}

// FireBallManager Ŭ������ ������Ʈ �Լ�
void FireBallManager::Update()
{
    // �� FireBall ��ü�� ������Ʈ�մϴ�.
    for (FireBall* fireBall : fireBalls)
        fireBall->Update();

    // ModelInstancing�� ������Ʈ�մϴ�.
    modelInstancing->Update();
}

// FireBallManager Ŭ������ ������ �Լ�
void FireBallManager::Render()
{
    // ModelInstancing�� ���� �ν��Ͻ̵� ��ü���� �������մϴ�.
    modelInstancing->Render();

    // �� FireBall ��ü�� �������մϴ�.
    for (FireBall* fireBall : fireBalls)
        fireBall->Render();
}

// FireBallManager Ŭ������ GUI ������ �Լ�
void FireBallManager::GUIRender()
{
    // ModelInstancing�� ���� �ν��Ͻ̵� ��ü���� GUI�� �������մϴ�.
    modelInstancing->GUIRender();

    // �� FireBall ��ü�� GUI�� �������մϴ�.
    for (FireBall* fireBall : fireBalls)
        fireBall->GUIRender();
}

// ���ο� FireBall�� �߻��ϴ� �Լ�
void FireBallManager::Fire(Vector3 pos, Vector3 direction)
{
    // ��Ȱ��ȭ�� FireBall�� ã�� �߻��մϴ�.
    for (FireBall* fireBall : fireBalls)
    {
        if (!fireBall->IsActive())
        {
            fireBall->Fire(pos, direction);
            return;
        }
    }
}