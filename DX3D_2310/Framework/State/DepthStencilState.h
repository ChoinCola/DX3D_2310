#pragma once

// ���� ���ٽ� ���¸� �����ϴ� Ŭ�����Դϴ�.
class DepthStencilState
{
public:
    DepthStencilState();
    ~DepthStencilState();

    // ���� ���� ���ٽ� ���¸� �����մϴ�.
    void SetState();

    // ���� �׽����� Ȱ��ȭ �Ǵ� ��Ȱ��ȭ�մϴ�.
    void DepthEnable(bool value);

    // ���� ���ۿ� ���� ���� ����ũ�� �����մϴ�.
    void DepthWriteMask(D3D11_DEPTH_WRITE_MASK value);

    // ���� ���ٽ� ���°� ����Ǿ����� ǥ���մϴ�.
    void Changed();

private:
    // D3D11_DEPTH_STENCIL_DESC ����ü�� ���ǵ� ���� ���ٽ� ���¸� �����ϴ� ��� �����Դϴ�.
    D3D11_DEPTH_STENCIL_DESC desc = {};

    // ���� ������ ���� ���ٽ� ���¸� ��Ÿ���� �������̽� ��� �����Դϴ�.
    ID3D11DepthStencilState* state = nullptr;
};