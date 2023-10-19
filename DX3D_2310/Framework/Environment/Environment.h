#pragma once
class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void Update();
    void Set();

    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetPerspective();

    void CreateProjection();
    void CreateState();
private:
    void CamMove();
private:
    MatrixBuffer* viewBuffer;
    MatrixBuffer* projectionBuffer;

    SamplerState* samplerState;
    RasterizerState* rasterizerSate[2];

    bool isWireMode = false;
};