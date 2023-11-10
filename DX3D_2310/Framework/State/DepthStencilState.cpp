#include "Framework.h"

// ������: �ʱ� ���� ���ٽ� ���¸� �����մϴ�.
DepthStencilState::DepthStencilState()
{
    desc.DepthEnable = true;  // ���� �׽����� Ȱ��ȭ�մϴ�.
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  // ��� ���� ���� ������ �����մϴ�.
    desc.DepthFunc = D3D11_COMPARISON_LESS;  // ���� �׽�Ʈ�� �� �Լ��� �����մϴ�.

    // ���ٽ� ���¸� �����մϴ�.
    desc.StencilEnable = true;
    desc.StencilReadMask = 0xff;
    desc.StencilWriteMask = 0xff;

    // ������������ ����
 
    // �ո� �� �޸��� ���ٽ� ���۷��̼� �� �� �Լ��� �����մϴ�.
    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    // ���ٽ� �׽�Ʈ�� �������� �� �� ó�� = ���н� ���� ���ٽ� ����
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    // ���ٽ� �׽�Ʈ�� ���������, �����׽�Ʈ�� �����Ѱ�� = ���ٽǰ��� ������Ŵ
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    // ���ٽ� �׽�Ʈ�� �����Ѱ�� = ���ٽǰ��� ����
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // ���ٽ� �׽�Ʈ�� ���Ǵ� ���Լ� = �׻󼺰���.

    // ������������ �޸�
    desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // ���� ���ٽ� ���°� ����Ǿ����� ǥ���մϴ�.
    Changed();
}

// �Ҹ���: �Ҵ�� ���ҽ��� �����մϴ�.
DepthStencilState::~DepthStencilState()
{
    state->Release();
}

// ���� ���� ���ٽ� ���¸� �����մϴ�.
void DepthStencilState::SetState()
{
    // 2DObject��±��� �ִ���̷� �����ع�����.
    // �̷����ν� ��� ��ü�� ������ ��ġ���� ����� �� �ְԵȴ�.
    DC->OMSetDepthStencilState(state, 1);
}

// ���� �׽����� Ȱ��ȭ �Ǵ� ��Ȱ��ȭ�մϴ�.
void DepthStencilState::DepthEnable(bool value)
{
    // ���̽��ٽ��� ����Ұ� ���� �ƴ��� ������.
    desc.DepthEnable = value;  // ���̹��۸� �Ѱų� ������ �����մϴ�.

    // ���� ���ٽ� ���°� ����Ǿ����� ǥ���մϴ�.
    Changed();
}

// ���� ���ۿ� ���� ���� ����ũ�� �����մϴ�.
void DepthStencilState::DepthWriteMask(D3D11_DEPTH_WRITE_MASK value)
{
    /*
    D3D11_DEPTH_WRITE_MASK_ZERO: 
    ���� ���ۿ� �ƹ��͵� ���� ����. ���Ⱑ ��Ȱ��ȭ�Ǹ�, 
    ���� ������ ������Ʈ���� �ʽ��ϴ�.

    D3D11_DEPTH_WRITE_MASK_ALL: 
    ��� �ȼ��� ���� ������ ���� ���ۿ� ���� ���� �����. 
    ���� ������ ������Ʈ�˴ϴ�.
    */
    desc.DepthWriteMask = value;

    // ���� ���ٽ� ���°� ����Ǿ����� ǥ���մϴ�.
    Changed();
}

// ���� ���ٽ� ���°� ����Ǿ��� �� ȣ��Ǹ�, ����� ���¸� �����մϴ�.
void DepthStencilState::Changed()
{
    if (state != nullptr)
        state->Release();  // ���� ���¸� �����մϴ�.

    // ���ο� ���� ���ٽ� ���¸� �����Ͽ� �����մϴ�.
    DEVICE->CreateDepthStencilState(&desc, &state);
}
