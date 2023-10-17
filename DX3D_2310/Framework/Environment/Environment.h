#pragma once
class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void Set();

    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetPerspective();

    void CreateProjection();
private:
    MatrixBuffer* viewBuffer;
    MatrixBuffer* projectionBuffer;
};