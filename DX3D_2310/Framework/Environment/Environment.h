#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void Update();
    void GUIRender();

    void Set();

    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetPerspective();

    void CreateProjection();
    void CreateState();
    float& GetCamSpeed() { return CamSpeed; }

    Camera* GetCamera() { return mainCamera; }

private:
    //void CamMove();

private:
    MatrixBuffer* projectionBuffer;

    SamplerState* samplerState;
    RasterizerState* rasterizerSate[2];

    Vector3 mousemovevalue = {};
    Camera* mainCamera;
    LightBuffer* lightBuffer;

    bool isWireMode = false;
    float CamSpeed = 20;
};