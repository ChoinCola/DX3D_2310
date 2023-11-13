#include "Framework.h"

DepthBuffer::DepthBuffer()
{
	// Buffer ����
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	// ���� ����(Depth Buffer)�� �ʺ�� ���̸� ����
	descDepth.Width = WIN_WIDTH;
	descDepth.Height = WIN_HEIGHT;

	// �̸� ���� �̿�� ������ ��
	descDepth.MipLevels = 1;
	// �ؽ�ó �迭�� ũ��
	descDepth.ArraySize = 1;
	// �ؽ�ó ���� (DXGI_FORMAT_D24_UNORM_S8_UINT�� 24��Ʈ ���̿� 8��Ʈ ���ٽ��� ����)
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// ���� ���ø��� ���� ���� ǰ�� ����
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;

	// ���� ���� (D3D11_USAGE_DEFAULT�� GPU���� �а� �� �� �ִ� �⺻ ����)
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	// ���ε� �÷��� (���� ���۷� ����� ���̹Ƿ� D3D11_BIND_DEPTH_STENCIL ����)
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// CPU���� �׼����� �ʿ䰡 �����Ƿ� 0���� ����
	descDepth.CPUAccessFlags = 0;
	// ��Ÿ �÷��� (��Ÿ Ư���� ������ �����Ƿ� 0���� ����)
	descDepth.MiscFlags = 0;
	// D3D11_TEXTURE2D_DESC ����ü�� ����Ͽ� ���� ���۸� ����
	DEVICE->CreateTexture2D(&descDepth, NULL, &DepthStencil);

	// Depth Buffer ���ø�
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	// ���̴� ���ҽ� �� ������ ���� (DXGI_FORMAT_R24_UNORM_X8_TYPELESS�� 24��Ʈ ���̸� ��Ÿ���� X8_TYPELESS�� ���ٽ� ����)
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // ���� ���ٽ� �����̽��� ����
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // ���� ������ �� ���� (2D �ؽ�ó)
	dsvDesc.Texture2D.MipSlice = 0; // �̿�� ����
	// ���̴� ���ҽ� �並 ����
	DEVICE->CreateDepthStencilView(DepthStencil, &dsvDesc, &DepthStencilView);
}

DepthBuffer::~DepthBuffer()
{
	SAFE_DELETE(DepthStencil);
	SAFE_DELETE(srv);
}

void DepthBuffer::SetPS(UINT slot)
{
	DC->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
	DC->OMSetDepthStencilState(DepthStencilState, 1);

}
