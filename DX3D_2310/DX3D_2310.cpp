﻿// DX3D_2310.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "DX3D_2310.h"

#define MAX_LOADSTRING 100

// 정점(Vertex) : 3차원 공간에 있는 한 점
struct Vertex
{
    XMFLOAT3 pos;
    
    Vertex(float x, float y)
        : pos(x, y, 0)
    {
        
    }
};


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND hWnd;

ID3D11Device* device;   // CPU
ID3D11DeviceContext* deviceContext; // GPU
// 하드웨어 정보부터 알아야 DX를 할 수 있다.
// CPU와 GPU가 있다. CPU와 GPU를 연결한다.

IDXGISwapChain* swapChain; // 백버퍼 관리
ID3D11RenderTargetView* renderTargetView; // GPU쪽 관련 메모리를 접근하는 용도. 중재역활
// 렌더타겟뷰 = 도화지를 만들어서 스왑체인형태로 만드는것.

ID3D11VertexShader* vertexShader; // 버텍스 쉐이더
ID3D11PixelShader* pixelShader;     // 픽셀 쉐이더
ID3D11InputLayout* inputLayout;     // 인풋 레이아웃
ID3D11Buffer* vertexBuffer;         // 버텍스 하나 넘겨서 출력이 가능하다.

void InitDevice();      // 시도
void Render();          // 렌더
void ReleaseDevice();   // 메모리 해제하는 것.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DX3D2310, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX3D2310));

    MSG msg = {};
    InitDevice();
    // 기본 메시지 루프입니다:
    while (msg.message != WM_QUIT) // 메세지에 종료가 들어오지 않으면 true를 반환한다.
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // 메세지 들어왔을때만 처리하고 아닐때는 버림.
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }   // 게임프로그래밍에서 많이 쓰는 루프방법.
        else
        {
            // Update
            // 이러면 프레임이 잘 나온다.
            Render();
        }

    }
    ReleaseDevice();

    return (int) msg.wParam;
}



void InitDevice()
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
        nullptr,                    // 1. 어답터 : 창관리, 창화면의 사이즈를 바꾸어 줄 수 있는것.
        D3D_DRIVER_TYPE_HARDWARE,   // 2. 드라이버 타입 : 하드웨어 가상X
        0,                          // 3. 소프트웨어 타입 : 가상 하드웨어
        D3D11_CREATE_DEVICE_DEBUG,  // 4. 플레그 : 다이렉트x 버전
        nullptr,                    // 5. 피쳐레벨 : 하드웨어 레벨 쓸껀지 
        0,                          // 6. 피쳐레벨 : 하드웨어 레벨 성능이 어느정도인지? 안씀 0임
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        nullptr,
        &deviceContext
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
    /////////////////////////////////////////////////////////////////////////////////

    D3D11_VIEWPORT viewPort;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    viewPort.Width = WIN_WIDTH;
    viewPort.Height = WIN_HEIGHT;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;

    deviceContext->RSSetViewports(1, &viewPort);

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    // 쉐이더 로드하는 일반적인 옵션이다.

    ID3DBlob* blob;
    // 로드하는 그래픽의 데이터
    D3DCompileFromFile(L"Shaders/Tutorial.hlsl", nullptr, nullptr,
        "VS", "vs_5_0", flags, 0, &blob, nullptr);
    
    device->CreateVertexShader
    (blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = 
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
        D3D11_INPUT_PER_VERTEX_DATA, 0}
    };  // 인스턴싱할때 바꾼다.
    
    UINT layoutSize = ARRAYSIZE(layoutDesc);

    device->CreateInputLayout(layoutDesc, layoutSize, 
        blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
    
    blob->Release();

    D3DCompileFromFile(L"Shaders/Tutorial.hlsl", nullptr, nullptr,
        "PS", "ps_5_0", flags, 0, &blob, nullptr);

    Vertex vertex(0,0); // 이대로는 못한다 vram에 넘겨야함

    { // VertexBuffer
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT; // 허가권한
        bufferDesc.ByteWidth = sizeof(Vertex);
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA subData = {};
        subData.pSysMem = &vertex;

        device->CreateBuffer(&bufferDesc, &subData, &vertexBuffer);
    }
    
}

void Render()
{
    float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
    // 디바이스 컨텍스트, 렌더타겟뷰를 컬러로 초기화 하겠다.

    //Render
    UINT stride = sizeof(Vertex); // 정점 하나
    UINT offset = 0; // 어디서부터 잡을껀지

    deviceContext->IASetInputLayout(inputLayout);
    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); // 점밖에 못그림

    deviceContext->VSSetShader(vertexShader, nullptr, 0);
    deviceContext->PSSetShader(pixelShader, nullptr, 0);

    deviceContext->DrawIndexed(1, 0, 0);

    swapChain->Present(0, 0);
}

void ReleaseDevice()
{
    device->Release();
    deviceContext->Release();

    swapChain->Release();
    renderTargetView->Release();
    // 할당된 것들을 전부 릴리즈 해준다.
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX3D2310));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DX3D2310);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) // 시작부분 중요한 부분
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   RECT rc = {0, 0, WIN_WIDTH, WIN_HEIGHT};
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false); // 윈도우의 Rect를 조절하겠다. Adjust = 조절하다.
   // LPRect = RECT*
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,// 전역으로 사용한다.
   // WIN_START_X, WIN_START_Y,  // 윈도우 창 시작위치 잡아주는것
      CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
    
   SetMenu(hWnd, nullptr); // 윈도우 창의 메뉴를 사용하지 않겠다.
   // hWnd핸들을 넣어서 해당 매뉴의 매뉴를 띄우지 않겠다.

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
