#include "Framework.h"

BlendState::BlendState()
{    
    // 블렌딩 기본설정
    desc.RenderTarget[0].BlendEnable = false;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;        

    Changed();
}

BlendState::~BlendState()
{
    state->Release();
}

// 블렌딩 관련함수 OM DEVICE_CONTEXT에 전달함. 0xffffffff 인 이유는 화면 전체에 마스크를 적용함.
// 0xf00000로 적으면 24비트 중 상위 4비트가 1로 설정되어 있으므로, 상위 4비트가 1인 픽셀만 스텐실 작업 또는 테스트를 통과 한다는 의미
void BlendState::SetState()
{
    float blendFactor[4] = {};
    DC->OMSetBlendState(state, blendFactor, 0xffffffff);
}
// 블렌딩 관련 함수 사전설정 데이터.
void BlendState::Alpha(bool value)
{
    desc.RenderTarget[0].BlendEnable = value;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    Changed();
}

void BlendState::Additive()
{
    desc.RenderTarget[0].BlendEnable = true;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    Changed();
}

void BlendState::AlphaToCoverage(bool value)
{
    desc.AlphaToCoverageEnable = value;

    Changed();
}

void BlendState::Changed()
{
    // 버퍼 데이터를 DEVICE에 전달하여 갱신한다.
    if (state != nullptr)
        state->Release();

    DEVICE->CreateBlendState(&desc, &state);
}
