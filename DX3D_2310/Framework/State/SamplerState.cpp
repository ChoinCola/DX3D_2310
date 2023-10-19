#include "Framework.h"

SamplerState::SamplerState()
{
    // 안티엘리어싱 할 때 어떤식으로 처리할것인지.

    /*
        D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR	= 0x5,
        선형보간으로 이미지의 픽셀 주변이 흐릿하게 처리해줌. 안티엘리어싱
        D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT	= 0x10,
        픽셀마다 딱딱 끊기게 흐릿하게 보간하지 않는다.

        3D에서는 리니어를 많이씀
        2D에서는 포인트를 많이씀

        D3D11_TEXTURE_ADDRESS_WRAP
        uv가 넘쳤을 때. 0에서 1까지 인데, 0부터 10까지 잡아서 넘치게 되었을 때
    */

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;
    //LOD(Level Of Detail) : 카메라의 거리에 따라서 퀄리티를 다르게 하는 것   
    // 최적화 기법중 하나임
    /*
        D3D11_TEXTURE_ADDRESS_WRAP	= 1,
        타일맵처럼 여러개를 찍어준다.
        D3D11_TEXTURE_ADDRESS_MIRROR	= 2,
        // 거울처럼 뒤집어주는것. uv가 넘어가면 뒤집어준다.
        D3D11_TEXTURE_ADDRESS_CLAMP	= 3,
        // 끝에서 끊어서 다시 역순으로 uv를 밀어넣어준다.
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
{   // UI출력할때 사용하는것들.
    desc.Filter = value;

    Changed();
}

void SamplerState::Changed()
{
    if (state != nullptr)
        state->Release();

    DEVICE->CreateSamplerState(&desc, &state);
}
