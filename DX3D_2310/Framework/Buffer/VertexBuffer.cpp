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

VertexBuffer::~VertexBuffer()
{
    buffer->Release();
}

void VertexBuffer::Set(D3D11_PRIMITIVE_TOPOLOGY type)
{
    DC->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    DC->IASetPrimitiveTopology(type);
}
