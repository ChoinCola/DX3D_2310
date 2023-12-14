#include "Framework.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
	: data(data), dataSize(dataSize)
{
    //ConstantBuffer
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = dataSize;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    DEVICE->CreateBuffer(&bufferDesc, nullptr, &buffer);
}

ConstBuffer::~ConstBuffer()
{
    buffer->Release();
}

void ConstBuffer::SetVS(UINT slot)
{
    // 말이 상수버퍼이지 cpu에서 사용할때 에는 cpu에서 큐브 돌릴때의 월드행렬이 들어있다.
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);
    // subResouce에 GPU주소값이 들어간다. 여기에 데이터를 밀어넣어줄 수 있다.
    memcpy(subResouce.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->VSSetConstantBuffers(slot, 1, &buffer); // slot = hlsl slot number
}

void ConstBuffer::SetPS(UINT slot)
{
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);
    // subResouce에 GPU주소값이 들어간다. 여기에 데이터를 밀어넣어줄 수 있다.
    memcpy(subResouce.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->PSSetConstantBuffers(slot, 1, &buffer); // slot = hlsl slot number
}

void ConstBuffer::SetCS(UINT slot)
{
    //DC->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);
    // subResouce에 GPU주소값이 들어간다. 여기에 데이터를 밀어넣어줄 수 있다.
    memcpy(subResouce.pData, data, dataSize);
    DC->Unmap(buffer, 0);

    DC->CSSetConstantBuffers(slot, 1, &buffer); // slot = hlsl slot number
}
