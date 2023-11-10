#include "Framework.h"

Font::Font()
{
	// 폰트 관련 작업을 처리하기 위한 DirectWrite 팩토리 생성
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&writeFactory);

	// 그래픽 렌더링을 위한 Direct2D 팩토리 생성
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);

	// 응용 프로그램의 디바이스로부터 DXGI 디바이스 획득
	IDXGIDevice* dxgiDevice;
	DEVICE->QueryInterface(&dxgiDevice);
	//디바이스를 쓰기위해 받아옴.

	// DXGI 디바이스를 사용하여 Direct2D 디바이스 생성
	factory->CreateDevice(dxgiDevice, &device);

	// Direct2D 디바이스 컨텍스트 생성
	device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &context);

	// 스왑 체인에서 DXGI 서피스 획득
	IDXGISurface* dxgiSurface;
	Device::Get()->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiSurface);

	// 비트맵 속성 설정
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	// DXGI 서피스로부터 Direct2D 비트맵 생성
	context->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap);

	// 렌더링 대상을 설정
	context->SetTarget(targetBitmap);
}

Font::~Font()
{
	// 컬러
	for (pair<string, ID2D1SolidColorBrush*> brush : brushes)
		brush.second->Release();

	// 폰트
	for (pair<string, IDWriteTextFormat*> format : formats)
		format.second->Release();

	factory->Release();
	writeFactory->Release();
	targetBitmap->Release();
	context->Release();
	device->Release();
}

void Font::AddColor(string key, float r, float g, float b)
{
	// 키(key)에 해당하는 브러시가 이미 존재하면 중복 생성을 피하기 위해 체크
	if (brushes.count(key) > 0) return;

	// D2D1::ColorF를 사용하여 RGB 값을 통해 ColorF 객체를 생성
	D2D1::ColorF colorF(r, g, b);

	// Solid Color 브러시를 생성하여 colorF로 색상을 설정
	ID2D1SolidColorBrush* brush = nullptr;
	context->CreateSolidColorBrush(colorF, &brush);

	// 생성된 브러시를 brushes 맵에 추가
	brushes[key] = brush;
}

void Font::AddStyle(string key, wstring font, float size, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
	// 키(key)에 해당하는 포맷이 이미 존재하면 중복 생성을 피하기 위해 체크
	if (formats.count(key) > 0) return;

	// IDWriteTextFormat 포맷을 생성
	IDWriteTextFormat* format;

	// CreateTextFormat 메서드를 사용하여 텍스트 포맷을 생성하고 초기화
	writeFactory->CreateTextFormat(font.c_str(), nullptr, weight, style, stretch, size, L"ko", &format);

	// 생성된 포맷을 formats 맵에 추가
	formats[key] = format;
}

void Font::SetColor(string key)
{
	if (brushes.count(key) == 0) return;

	curBrush = brushes[key];
}

void Font::SetStyle(string key)
{
	if (formats.count(key) == 0) return;

	curFormat = formats[key];
}

void Font::RenderText(wstring text, Float2 pos, Float2 size)
{
	// 만약 크기가 0인 경우, 텍스트의 길이와 현재 포맷의 폰트 크기를 곱하여 크기를 설정
	if (size.x == 0.0f && size.y == 0.0f)
	{
		size.x = text.size() * curFormat->GetFontSize();
		size.y = curFormat->GetFontSize();
	}

	// 크기의 반을 계산하여 halfSize에 저장
	Float2 halfSize = { size.x * 0.5f, size.y * 0.5f };

	// Y 좌표를 윈도우 높이에서 빼서 좌표 공간을 조정
	pos.y = WIN_HEIGHT - pos.y;

	// D2D1_RECT_F 구조체를 사용하여 텍스트를 그릴 영역을 정의
	D2D1_RECT_F rectF;
	rectF.left = pos.x - halfSize.x;
	rectF.right = pos.x + halfSize.x;
	rectF.top = pos.y - halfSize.y;
	rectF.bottom = pos.y + halfSize.y;

	// 설정된 영역에 텍스트를 그림
	context->DrawTextW(text.c_str(), text.size(), curFormat, &rectF, curBrush);
}

void Font::RenderText(string text, Float2 pos, Float2 size)
{
	// 만약 크기가 0인 경우, 텍스트의 길이와 현재 포맷의 폰트 크기를 곱하여 크기를 설정
	if (size.x == 0.0f && size.y == 0.0f)
	{
		size.x = text.size() * curFormat->GetFontSize();
		size.y = curFormat->GetFontSize();
	}

	// 크기의 반을 계산하여 halfSize에 저장
	Float2 halfSize = { size.x * 0.5f, size.y * 0.5f };

	// Y 좌표를 윈도우 높이에서 빼서 좌표 공간을 조정
	pos.y = WIN_HEIGHT - pos.y;

	// D2D1_RECT_F 구조체를 사용하여 텍스트를 그릴 영역을 정의
	D2D1_RECT_F rectF;
	rectF.left = pos.x - halfSize.x;
	rectF.right = pos.x + halfSize.x;
	rectF.top = pos.y - halfSize.y;
	rectF.bottom = pos.y + halfSize.y;

	// ANSI 문자열을 wide 문자열로 변환
	wstring temp = ChangeWString(text);

	// 설정된 영역에 변환된 wide 문자열로 텍스트를 그림
	context->DrawTextW(temp.c_str(), temp.size(), curFormat, &rectF, curBrush);
}

wstring Font::ChangeWString(string value)
{
	// 필요한 버퍼의 길이 계산
	int nLen = MultiByteToWideChar(CP_ACP, 0, &value[0], value.size(), nullptr, 0);

	// wide 문자열을 저장할 wstring 생성
	wstring strUni(nLen + 1, 0);

	// ANSI 문자열을 wide 문자열로 변환
	int len = MultiByteToWideChar(CP_ACP, 0, value.c_str(), value.size(), &strUni[0], nLen);

	// Null 종료 문자 추가
	strUni[len] = 0;

	// 변환된 wide 문자열 반환
	return strUni;
}
