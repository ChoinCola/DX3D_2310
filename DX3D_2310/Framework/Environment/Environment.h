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
    void SetPost();

    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);

    void CreateProjection();
    void CreateState();
    float& GetCamSpeed() { return CamSpeed; }
    bool GetIsPrintMinimap() { return IsPrintMinimap; }
    Camera* GetMainCamera() { return mainCamera; }

    Matrix GetProjection() { return projection; }

private:
    void EditLight(LightBuffer::Light* light, int index);

private:
    MatrixBuffer* projectionBuffer;
    ViewBuffer* uiViewBuffer;

    SamplerState* samplerState;
    RasterizerState* rasterizerSate[2];
    BlendState* blendState[2];

    Vector3 mousemovevalue = {};

    Camera* mainCamera;
    LightBuffer* lightBuffer;

    Matrix projection;
    Matrix orthograpic;

    bool isWireMode = false;
    bool IsPrintMinimap = false;
    float CamSpeed = 20;
};