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
    // 할당된 것들을 전부 릴리즈 해준다.
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
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {}; // DESC = Discripstion. 표현하다. 스왑체인을 표현하다.
    swapChainDesc.BufferDesc.Width = WIN_WIDTH; // 디바이스 크기
    swapChainDesc.BufferDesc.Height = WIN_HEIGHT; // 디바이스 크기
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // 프레임
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // 프레임 디폴트값으로 저장해준다.
    // RefreshRate : 디스플레이 모드 갱신률(주시율 : Numerate / Denominator)
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;   // 표본화 라는 개념.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = true;
    // 표본화 : 화면을 확대하거나 줄이면 이미지가 꺠진다. 이를 방지하기 위해
    // 아예 해상도가 높게 저장한다음에 줄일때만 해상도를 죽이는 느낌으로
    // 그런데 용량이 너무 큰 문제가 있음
    // DX에서는 다중 표본화 라는 기술을 사용한다.
    // 여러개의 이미지를 가지게한다음에 화소가 바뀔때 다른걸 넣는다.
    // 그런데 게임프로그래밍에서는 이걸 안쓴다.
    // 1이라는건 이걸 안쓰고 1개의 이미지만 사용한다는것.

    D3D11CreateDeviceAndSwapChain(
        nullptr,                    // 1. 어답터 : 창관리어뎁터, null이 들어가면 기본이 사용됨
        D3D_DRIVER_TYPE_HARDWARE,   // 2. 드라이버 타입 : 하드웨어 인지 가상드라이브인지 등 을 설정한다.
        0,                          // 3. 소프트웨어 타입 : 드라이버타입 에서 소프트웨어 유형을 설정했을때만 지정한다.
        D3D11_CREATE_DEVICE_DEBUG,  // 4. 플레그 : 현재 진행하는 디바이스가 디버그인지 일반인지 지정한다.
        nullptr,                    // 5. 피쳐레벨 : 어플리케이션이 하드웨어 성능을 얼마나 쓰는지 지정
        0,                          // 6. 피쳐레벨 : 위 값이 있을경우 배열의 개수를 지정한다.
        D3D11_SDK_VERSION,          // 7. SDK버전 : 현재 SDK버전 을 기입한다.
        &swapChainDesc,             // 8. 스왑체인 : 현재 스왑체인의 기입사항을 지정해준다.
        &swapChain,                 // 9. 스왑체인인터페이스 : 인터페이스 포인터를 기입한다.
        &device,                    // 10. 디바이스(CPU) : CPU 인터페이스 포인터를 기입한다.
        nullptr,                    // 11. 디바이스수준 : CPU기능수준을 기입한다. null일경우 사용X
        &deviceContext              // 12. 디바이스(GPU) : GPU 인터페이스 포인터를 기입한다.
    );

    ID3D11Texture2D* backBuffer;
    //백버퍼 정보는 스왑체인이 들고있다.

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    // IID = interface ID
    // 보이드포인터 = 버튼을 받는데 어떤 버튼인지 모를때 voidpointer를 사용한다.
    // 보이드포인터 이기 때문에 뭐를 받는지 몰라서. 그 포인터의 IID를 받아오는 것이다.
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    // 렌더타겟뷰를 백버퍼로 사용한다. 렌더타겟뷰의 주소를 전달.
    backBuffer->Release();
    // 벡버퍼를 그래픽카드로 넘겨주면 쓸모가 없기 떄문에 삭제해준다.
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
    // 3D를 넣으려면 깊이값도 넣어야 하지만, nullptr로 아직 넘어간다. 아웃펏 머지 설정한것
}
