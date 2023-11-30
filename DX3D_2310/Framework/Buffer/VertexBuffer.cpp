#include "Framework.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count)
	: stride(stride)
{
    //VertexBuffer
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = stride * count;                  // 옮길 사이즈
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;        //용도

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem = data;                                 // 렘의 데이터 위치값

    DEVICE->CreateBuffer(&bufferDesc, &subData, &buffer);
}

// VertexBuffer 클래스의 소멸자
// VertexBuffer와 관련된 Direct3D 버퍼를 해제합니다.
VertexBuffer::~VertexBuffer()
{
    buffer->Release();
}

// 버텍스 버퍼를 설정하고, 렌더링에 사용할 프리미티브 토폴로지를 지정합니다.
// 매개변수:
//   type - 설정할 프리미티브 토폴로지 (예: D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
void VertexBuffer::Set(D3D11_PRIMITIVE_TOPOLOGY type)
{
    // 입력 어셈블러 스테이지에 대한 버텍스 버퍼, 스트라이드 및 오프셋을 설정합니다.
    DC->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    // 입력 어셈블러 스테이지에 대한 프리미티브 토폴로지를 설정합니다.
    DC->IASetPrimitiveTopology(type);
}

// 특정 슬롯에 대한 버텍스 버퍼를 설정하고, 렌더링에 사용할 프리미티브 토폴로지를 지정합니다.
// 매개변수:
//   slot - 설정할 슬롯 번호
//   type - 설정할 프리미티브 토폴로지 (예: D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
void VertexBuffer::Set(UINT slot, D3D11_PRIMITIVE_TOPOLOGY type)
{
    // 입력 어셈블러 스테이지에 대한 특정 슬롯에 대한 버텍스 버퍼, 스트라이드 및 오프셋을 설정합니다.
    DC->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
    // 입력 어셈블러 스테이지에 대한 프리미티브 토폴로지를 설정합니다.
    DC->IASetPrimitiveTopology(type);
}

// 버텍스 버퍼의 데이터를 업데이트합니다.
// 매개변수:
//   data - 업데이트할 데이터의 포인터
//   count - 업데이트할 요소의 수
void VertexBuffer::Update(void* data, UINT count)
{
    // Direct3D 버퍼를 업데이트합니다.
    DC->UpdateSubresource(buffer, 0, nullptr, data, stride, count);
}