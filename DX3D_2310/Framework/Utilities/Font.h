#pragma once

class Font : public Singleton<Font>
{
public:
    Font();
    ~Font();

    // 색상을. 텍스트처럼 쉐이더처럼 사용하는것.
    void AddColor(string key, float r, float g, float b);
    void AddStyle(string key, wstring font, float size = 20.0f,
        DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, // 사이즈
        DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL,// 폰트 기울이기등
        DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL); // 밑줄 가운뎃줄

    void SetColor(string key);
    void SetStyle(string key);

    void RenderText(wstring text, Float2 pos, Float2 size = Float2());
    void RenderText(string text, Float2 pos, Float2 size = Float2());

    ID2D1DeviceContext* GetDC() { return context; }

    wstring ChangeWString(string value);

private:
    ID2D1Factory1* factory;
    IDWriteFactory* writeFactory;

    // 2D Device.
    ID2D1Device* device;
    ID2D1DeviceContext* context;

    ID2D1Bitmap1* targetBitmap;

    map<string, ID2D1SolidColorBrush*> brushes;
    map<string, IDWriteTextFormat*> formats;

    ID2D1SolidColorBrush* curBrush;
    IDWriteTextFormat* curFormat;
};