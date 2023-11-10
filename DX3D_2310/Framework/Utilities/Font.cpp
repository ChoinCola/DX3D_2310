#include "Framework.h"

Font::Font()
{
	// ��Ʈ ���� �۾��� ó���ϱ� ���� DirectWrite ���丮 ����
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&writeFactory);

	// �׷��� �������� ���� Direct2D ���丮 ����
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);

	// ���� ���α׷��� ����̽��κ��� DXGI ����̽� ȹ��
	IDXGIDevice* dxgiDevice;
	DEVICE->QueryInterface(&dxgiDevice);
	//����̽��� �������� �޾ƿ�.

	// DXGI ����̽��� ����Ͽ� Direct2D ����̽� ����
	factory->CreateDevice(dxgiDevice, &device);

	// Direct2D ����̽� ���ؽ�Ʈ ����
	device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &context);

	// ���� ü�ο��� DXGI ���ǽ� ȹ��
	IDXGISurface* dxgiSurface;
	Device::Get()->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiSurface);

	// ��Ʈ�� �Ӽ� ����
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	// DXGI ���ǽ��κ��� Direct2D ��Ʈ�� ����
	context->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap);

	// ������ ����� ����
	context->SetTarget(targetBitmap);
}

Font::~Font()
{
	// �÷�
	for (pair<string, ID2D1SolidColorBrush*> brush : brushes)
		brush.second->Release();

	// ��Ʈ
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
	// Ű(key)�� �ش��ϴ� �귯�ð� �̹� �����ϸ� �ߺ� ������ ���ϱ� ���� üũ
	if (brushes.count(key) > 0) return;

	// D2D1::ColorF�� ����Ͽ� RGB ���� ���� ColorF ��ü�� ����
	D2D1::ColorF colorF(r, g, b);

	// Solid Color �귯�ø� �����Ͽ� colorF�� ������ ����
	ID2D1SolidColorBrush* brush = nullptr;
	context->CreateSolidColorBrush(colorF, &brush);

	// ������ �귯�ø� brushes �ʿ� �߰�
	brushes[key] = brush;
}

void Font::AddStyle(string key, wstring font, float size, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
	// Ű(key)�� �ش��ϴ� ������ �̹� �����ϸ� �ߺ� ������ ���ϱ� ���� üũ
	if (formats.count(key) > 0) return;

	// IDWriteTextFormat ������ ����
	IDWriteTextFormat* format;

	// CreateTextFormat �޼��带 ����Ͽ� �ؽ�Ʈ ������ �����ϰ� �ʱ�ȭ
	writeFactory->CreateTextFormat(font.c_str(), nullptr, weight, style, stretch, size, L"ko", &format);

	// ������ ������ formats �ʿ� �߰�
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
	// ���� ũ�Ⱑ 0�� ���, �ؽ�Ʈ�� ���̿� ���� ������ ��Ʈ ũ�⸦ ���Ͽ� ũ�⸦ ����
	if (size.x == 0.0f && size.y == 0.0f)
	{
		size.x = text.size() * curFormat->GetFontSize();
		size.y = curFormat->GetFontSize();
	}

	// ũ���� ���� ����Ͽ� halfSize�� ����
	Float2 halfSize = { size.x * 0.5f, size.y * 0.5f };

	// Y ��ǥ�� ������ ���̿��� ���� ��ǥ ������ ����
	pos.y = WIN_HEIGHT - pos.y;

	// D2D1_RECT_F ����ü�� ����Ͽ� �ؽ�Ʈ�� �׸� ������ ����
	D2D1_RECT_F rectF;
	rectF.left = pos.x - halfSize.x;
	rectF.right = pos.x + halfSize.x;
	rectF.top = pos.y - halfSize.y;
	rectF.bottom = pos.y + halfSize.y;

	// ������ ������ �ؽ�Ʈ�� �׸�
	context->DrawTextW(text.c_str(), text.size(), curFormat, &rectF, curBrush);
}

void Font::RenderText(string text, Float2 pos, Float2 size)
{
	// ���� ũ�Ⱑ 0�� ���, �ؽ�Ʈ�� ���̿� ���� ������ ��Ʈ ũ�⸦ ���Ͽ� ũ�⸦ ����
	if (size.x == 0.0f && size.y == 0.0f)
	{
		size.x = text.size() * curFormat->GetFontSize();
		size.y = curFormat->GetFontSize();
	}

	// ũ���� ���� ����Ͽ� halfSize�� ����
	Float2 halfSize = { size.x * 0.5f, size.y * 0.5f };

	// Y ��ǥ�� ������ ���̿��� ���� ��ǥ ������ ����
	pos.y = WIN_HEIGHT - pos.y;

	// D2D1_RECT_F ����ü�� ����Ͽ� �ؽ�Ʈ�� �׸� ������ ����
	D2D1_RECT_F rectF;
	rectF.left = pos.x - halfSize.x;
	rectF.right = pos.x + halfSize.x;
	rectF.top = pos.y - halfSize.y;
	rectF.bottom = pos.y + halfSize.y;

	// ANSI ���ڿ��� wide ���ڿ��� ��ȯ
	wstring temp = ChangeWString(text);

	// ������ ������ ��ȯ�� wide ���ڿ��� �ؽ�Ʈ�� �׸�
	context->DrawTextW(temp.c_str(), temp.size(), curFormat, &rectF, curBrush);
}

wstring Font::ChangeWString(string value)
{
	// �ʿ��� ������ ���� ���
	int nLen = MultiByteToWideChar(CP_ACP, 0, &value[0], value.size(), nullptr, 0);

	// wide ���ڿ��� ������ wstring ����
	wstring strUni(nLen + 1, 0);

	// ANSI ���ڿ��� wide ���ڿ��� ��ȯ
	int len = MultiByteToWideChar(CP_ACP, 0, value.c_str(), value.size(), &strUni[0], nLen);

	// Null ���� ���� �߰�
	strUni[len] = 0;

	// ��ȯ�� wide ���ڿ� ��ȯ
	return strUni;
}
