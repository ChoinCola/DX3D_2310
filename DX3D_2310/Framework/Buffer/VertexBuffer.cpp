#include "Framework.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count)
	: stride(stride)
{
    //VertexBuffer
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = stride * count;                  // �ű� ������
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;        //�뵵

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem = data;                                 // ���� ������ ��ġ��

    DEVICE->CreateBuffer(&bufferDesc, &subData, &buffer);
}

// VertexBuffer Ŭ������ �Ҹ���
// VertexBuffer�� ���õ� Direct3D ���۸� �����մϴ�.
VertexBuffer::~VertexBuffer()
{
    buffer->Release();
}

// ���ؽ� ���۸� �����ϰ�, �������� ����� ������Ƽ�� ���������� �����մϴ�.
// �Ű�����:
//   type - ������ ������Ƽ�� �������� (��: D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
void VertexBuffer::Set(D3D11_PRIMITIVE_TOPOLOGY type)
{
    // �Է� ����� ���������� ���� ���ؽ� ����, ��Ʈ���̵� �� �������� �����մϴ�.
    DC->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    // �Է� ����� ���������� ���� ������Ƽ�� ���������� �����մϴ�.
    DC->IASetPrimitiveTopology(type);
}

// Ư�� ���Կ� ���� ���ؽ� ���۸� �����ϰ�, �������� ����� ������Ƽ�� ���������� �����մϴ�.
// �Ű�����:
//   slot - ������ ���� ��ȣ
//   type - ������ ������Ƽ�� �������� (��: D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
void VertexBuffer::Set(UINT slot, D3D11_PRIMITIVE_TOPOLOGY type)
{
    // �Է� ����� ���������� ���� Ư�� ���Կ� ���� ���ؽ� ����, ��Ʈ���̵� �� �������� �����մϴ�.
    DC->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
    // �Է� ����� ���������� ���� ������Ƽ�� ���������� �����մϴ�.
    DC->IASetPrimitiveTopology(type);
}

// ���ؽ� ������ �����͸� ������Ʈ�մϴ�.
// �Ű�����:
//   data - ������Ʈ�� �������� ������
//   count - ������Ʈ�� ����� ��
void VertexBuffer::Update(void* data, UINT count)
{
    // Direct3D ���۸� ������Ʈ�մϴ�.
    DC->UpdateSubresource(buffer, 0, nullptr, data, stride, count);
}