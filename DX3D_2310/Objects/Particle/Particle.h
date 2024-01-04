#pragma once

class Particle
{
protected:
    const UINT SIZE = 1000;

public:
    Particle();
    ~Particle();

    virtual void Update() {}
    virtual void Render();
    virtual void GUIRender() {}

    virtual void Play(Vector3 position);

    void Stop();

protected:
    BlendState* blendState[2];
    DepthStencilState* depthState[2];

    Material* material;
    VertexBuffer* vertexBuffer;
    GeometryShader* geometryShader;

    bool isActive = false;
    UINT particleCount = 0;
    Vector3 position;
};