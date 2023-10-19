#include "Framework.h"

SamplerState::SamplerState()
{
    // ��Ƽ������� �� �� ������� ó���Ұ�����.

    /*
        D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR	= 0x5,
        ������������ �̹����� �ȼ� �ֺ��� �帴�ϰ� ó������. ��Ƽ�������
        D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT	= 0x10,
        �ȼ����� ���� ����� �帴�ϰ� �������� �ʴ´�.

        3D������ ���Ͼ ���̾�
        2D������ ����Ʈ�� ���̾�

        D3D11_TEXTURE_ADDRESS_WRAP
        uv�� ������ ��. 0���� 1���� �ε�, 0���� 10���� ��Ƽ� ��ġ�� �Ǿ��� ��
    */

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;
    //LOD(Level Of Detail) : ī�޶��� �Ÿ��� ���� ����Ƽ�� �ٸ��� �ϴ� ��   
    // ����ȭ ����� �ϳ���
    /*
        D3D11_TEXTURE_ADDRESS_WRAP	= 1,
        Ÿ�ϸ�ó�� �������� ����ش�.
        D3D11_TEXTURE_ADDRESS_MIRROR	= 2,
        // �ſ�ó�� �������ִ°�. uv�� �Ѿ�� �������ش�.
        D3D11_TEXTURE_ADDRESS_CLAMP	= 3,
        // ������ ��� �ٽ� �������� uv�� �о�־��ش�.
        D3D11_TEXTURE_ADDRESS_BORDER	= 4,
        D3D11_TEXTURE_ADDRESS_MIRROR_ONCE	= 5
    
    */

    Changed();
}

SamplerState::~SamplerState()
{
    state->Release();
}

void SamplerState::SetState(UINT slot)
{
    DC->PSSetSamplers(slot, 1, &state);
    //DC->DSSetSamplers(slot, 1, &state);
}

void SamplerState::Filter(D3D11_FILTER value)
{   // UI����Ҷ� ����ϴ°͵�.
    desc.Filter = value;

    Changed();
}

void SamplerState::Changed()
{
    if (state != nullptr)
        state->Release();

    DEVICE->CreateSamplerState(&desc, &state);
}
