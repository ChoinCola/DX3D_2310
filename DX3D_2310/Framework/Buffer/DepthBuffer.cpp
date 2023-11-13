#include "Framework.h"

DepthBuffer::DepthBuffer()
{
	// Buffer 생성
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	// 깊이 버퍼(Depth Buffer)의 너비와 높이를 설정
	descDepth.Width = WIN_WIDTH;
	descDepth.Height = WIN_HEIGHT;

	// 미리 계산된 미웁맵 레벨의 수
	descDepth.MipLevels = 1;
	// 텍스처 배열의 크기
	descDepth.ArraySize = 1;
	// 텍스처 형식 (DXGI_FORMAT_D24_UNORM_S8_UINT는 24비트 깊이와 8비트 스텐실을 저장)
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 다중 샘플링의 샘플 수와 품질 레벨
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;

	// 버퍼 사용법 (D3D11_USAGE_DEFAULT은 GPU에서 읽고 쓸 수 있는 기본 사용법)
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	// 바인딩 플래그 (깊이 버퍼로 사용할 것이므로 D3D11_BIND_DEPTH_STENCIL 설정)
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// CPU에서 액세스할 필요가 없으므로 0으로 설정
	descDepth.CPUAccessFlags = 0;
	// 기타 플래그 (기타 특별한 설정은 없으므로 0으로 설정)
	descDepth.MiscFlags = 0;
	// D3D11_TEXTURE2D_DESC 구조체를 사용하여 깊이 버퍼를 생성
	DEVICE->CreateTexture2D(&descDepth, NULL, &DepthStencil);

	// Depth Buffer 샘플링
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	// 셰이더 리소스 뷰 형식을 설정 (DXGI_FORMAT_R24_UNORM_X8_TYPELESS는 24비트 깊이를 나타내며 X8_TYPELESS는 스텐실 형식)
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 깊이 스텐실 서페이스의 형식
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // 깊이 버퍼의 뷰 차원 (2D 텍스처)
	dsvDesc.Texture2D.MipSlice = 0; // 미웁맵 레벨
	// 셰이더 리소스 뷰를 생성
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
