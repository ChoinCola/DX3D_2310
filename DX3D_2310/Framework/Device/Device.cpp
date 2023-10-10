#include "Framework.h"

Device::Device()
{
	CreateDeviceAndSwapChain();
}

Device::~Device()
{
    device->Release();
    deviceContext->Release();

    swapChain->Release();
    renderTargetView->Release();
    // �Ҵ�� �͵��� ���� ������ ���ش�.
}

void Device::Clear()
{
    float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);


}

void Device::Present()
{
    swapChain->Present(0, 0);
}

void Device::CreateDeviceAndSwapChain()
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {}; // DESC = Discripstion. ǥ���ϴ�. ����ü���� ǥ���ϴ�.
    swapChainDesc.BufferDesc.Width = WIN_WIDTH; // ����̽� ũ��
    swapChainDesc.BufferDesc.Height = WIN_HEIGHT; // ����̽� ũ��
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // ������
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // ������ ����Ʈ������ �������ش�.
    // RefreshRate : ���÷��� ��� ���ŷ�(�ֽ��� : Numerate / Denominator)
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;   // ǥ��ȭ ��� ����.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = true;
    // ǥ��ȭ : ȭ���� Ȯ���ϰų� ���̸� �̹����� ������. �̸� �����ϱ� ����
    // �ƿ� �ػ󵵰� ���� �����Ѵ����� ���϶��� �ػ󵵸� ���̴� ��������
    // �׷��� �뷮�� �ʹ� ū ������ ����
    // DX������ ���� ǥ��ȭ ��� ����� ����Ѵ�.
    // �������� �̹����� �������Ѵ����� ȭ�Ұ� �ٲ� �ٸ��� �ִ´�.
    // �׷��� �������α׷��ֿ����� �̰� �Ⱦ���.
    // 1�̶�°� �̰� �Ⱦ��� 1���� �̹����� ����Ѵٴ°�.

    D3D11CreateDeviceAndSwapChain(
        nullptr,                    // 1. ����� : â�������, null�� ���� �⺻�� ����
        D3D_DRIVER_TYPE_HARDWARE,   // 2. ����̹� Ÿ�� : �ϵ���� ���� �������̺����� �� �� �����Ѵ�.
        0,                          // 3. ����Ʈ���� Ÿ�� : ����̹�Ÿ�� ���� ����Ʈ���� ������ ������������ �����Ѵ�.
        D3D11_CREATE_DEVICE_DEBUG,  // 4. �÷��� : ���� �����ϴ� ����̽��� ��������� �Ϲ����� �����Ѵ�.
        nullptr,                    // 5. ���ķ��� : ���ø����̼��� �ϵ���� ������ �󸶳� ������ ����
        0,                          // 6. ���ķ��� : �� ���� ������� �迭�� ������ �����Ѵ�.
        D3D11_SDK_VERSION,          // 7. SDK���� : ���� SDK���� �� �����Ѵ�.
        &swapChainDesc,             // 8. ����ü�� : ���� ����ü���� ���Ի����� �������ش�.
        &swapChain,                 // 9. ����ü���������̽� : �������̽� �����͸� �����Ѵ�.
        &device,                    // 10. ����̽�(CPU) : CPU �������̽� �����͸� �����Ѵ�.
        nullptr,                    // 11. ����̽����� : CPU��ɼ����� �����Ѵ�. null�ϰ�� ���X
        &deviceContext              // 12. ����̽�(GPU) : GPU �������̽� �����͸� �����Ѵ�.
    );

    ID3D11Texture2D* backBuffer;
    //����� ������ ����ü���� ����ִ�.

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    // IID = interface ID
    // ���̵������� = ��ư�� �޴µ� � ��ư���� �𸦶� voidpointer�� ����Ѵ�.
    // ���̵������� �̱� ������ ���� �޴��� ����. �� �������� IID�� �޾ƿ��� ���̴�.
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    // ����Ÿ�ٺ並 ����۷� ����Ѵ�. ����Ÿ�ٺ��� �ּҸ� ����.
    backBuffer->Release();
    // �����۸� �׷���ī��� �Ѱ��ָ� ���� ���� ������ �������ش�.
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
    // 3D�� �������� ���̰��� �־�� ������, nullptr�� ���� �Ѿ��. �ƿ��� ���� �����Ѱ�
}
