#include "Framework.h"

BlendState::BlendState()
{    
    // ���� �⺻����
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

// ���� �����Լ� OM DEVICE_CONTEXT�� ������. 0xffffffff �� ������ ȭ�� ��ü�� ����ũ�� ������.
// 0xf00000�� ������ 24��Ʈ �� ���� 4��Ʈ�� 1�� �����Ǿ� �����Ƿ�, ���� 4��Ʈ�� 1�� �ȼ��� ���ٽ� �۾� �Ǵ� �׽�Ʈ�� ��� �Ѵٴ� �ǹ�
void BlendState::SetState()
{
    float blendFactor[4] = {};
    DC->OMSetBlendState(state, blendFactor, 0xffffffff);
}
// ���� ���� �Լ� �������� ������.
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
    // ���� �����͸� DEVICE�� �����Ͽ� �����Ѵ�.
    if (state != nullptr)
        state->Release();

    DEVICE->CreateBlendState(&desc, &state);
}
