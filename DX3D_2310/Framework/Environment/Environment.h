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
    LightBuffer* GetLightBuffer() { return lightBuffer; }
    Matrix GetProjection() { return projection; }

    LightBuffer::Light* GetLight(int index) { return &lightBuffer->GetData()->lights[index]; }

    int SetLight() { 
        lightBuffer->GetData()->lightCount++; 
        return lightBuffer->GetData()->lightCount;
    }

private:
    void EditLight(LightBuffer::Light* light, int index);

private:
    MatrixBuffer* projectionBuffer;
    ViewBuffer* uiViewBuffer;

    SamplerState* samplerState;
    RasterizerState* rasterizerSate[2];
    BlendState* blendState[2];
    DepthStencilState* depthState[2];

    Vector3 mousemovevalue = {};

    Camera* mainCamera;
    LightBuffer* lightBuffer;

    Matrix projection;
    Matrix orthograpic;

    bool isWireMode = false;
    bool IsPrintMinimap = false;
    float CamSpeed = 20;
};