#include "Framework.h"

Buffer::Buffer(UINT BindFlags, UINT ByteWidth, D3D11_USAGE Usage, void* Vector_Start)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = Usage; // �㰡����
    bufferDesc.ByteWidth = ByteWidth;
    bufferDesc.BindFlags = BindFlags;

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem = Vector_Start;

    DEVICE->CreateBuffer(&bufferDesc, &subData, &buffer);
}

Buffer::~Buffer()
{
    delete layoutDesc;
    delete blob;
}

void Buffer::BindFlags(int BindFlags)
{
}

void Buffer::SetElementDesc()
{
    layoutDesc = new D3D11_INPUT_ELEMENT_DESC[2];
    {
        layoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D11_INPUT_PER_VERTEX_DATA, 0 };
        // ������ ��ǲ�� ����ش�.

        layoutDesc[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,// �տ� �����Ʈ�� �質? ǥ��
        D3D11_INPUT_PER_VERTEX_DATA, 0 }; // �÷� ��ǲ�� ����ش�
    };  // �ν��Ͻ��Ҷ� �ٲ۴�.
}