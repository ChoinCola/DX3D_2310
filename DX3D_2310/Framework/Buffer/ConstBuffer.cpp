#include "Framework.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
	: data(data), dataSize(dataSize)
{
    //ConstantBuffer
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = dataSize;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    DEVICE->CreateBuffer(&bufferDesc, nullptr, &buffer);
}

ConstBuffer::~ConstBuffer()
{
    buffer->Release();
}

void ConstBuffer::SetVS(UINT slot)
{
    // ���� ����������� cpu���� ����Ҷ� ���� cpu���� ť�� �������� ��������� ����ִ�.
    DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
    DC->VSSetConstantBuffers(slot, 1, &buffer); // slot = hlsl slot number
}

void ConstBuffer::SetPS(UINT slot)
{
    DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
    DC->PSSetConstantBuffers(slot, 1, &buffer); // slot = hlsl slot number
}
