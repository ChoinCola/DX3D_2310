#pragma once
// 정점(Vertex) : 3차원 공간에 있는 한 점


class Box : public Iobject 
{
public:
	Box(int size, XMFLOAT3 position);
    Box(int size, XMFLOAT3 position, XMFLOAT3 color);
	~Box();
public:
    // Iobject을(를) 통해 상속됨
    void Update() override;
    void Render() override;
    void Delete() override;

private:
    vector<Vertex> vertices;
    vector<UINT> indices;

    Buffer* vertexBuffer;
    Buffer* IndexBuffer;
    Shader* shader;
};