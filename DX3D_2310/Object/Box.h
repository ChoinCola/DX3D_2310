#pragma once
// ����(Vertex) : 3���� ������ �ִ� �� ��


class Box : public Iobject 
{
public:
	Box(int size, XMFLOAT3 position);
    Box(int size, XMFLOAT3 position, XMFLOAT3 color);
	~Box();
public:
    // Iobject��(��) ���� ��ӵ�
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