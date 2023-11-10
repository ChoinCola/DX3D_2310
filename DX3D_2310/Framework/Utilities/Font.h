#pragma once

class Font : public Singleton<Font>
{
public:
    Font();
    ~Font();

    // 텍스트의 색상을 추가하는 메서드. 쉐이더처럼 사용됩니다.
    void AddColor(string key, float r, float g, float b);

    // 텍스트 스타일을 추가하는 메서드. 기본값 설정 가능.
    void AddStyle(string key, wstring font, float size = 20.0f,
        DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, // 사이즈
        DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL,// 폰트 기울이기등
        DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL); // 밑줄 가운뎃줄

    // 지정된 키에 해당하는 색상을 설정합니다.
    void SetColor(string key);

    // 지정된 키에 해당하는 스타일을 설정합니다.
    void SetStyle(string key);

    // 텍스트를 렌더링하는 메서드. 위치와 크기 지정 가능.
    void RenderText(wstring text, Float2 pos, Float2 size = Float2());
    void RenderText(string text, Float2 pos, Float2 size = Float2());

    // 현재의 디바이스 컨텍스트를 반환합니다.
    ID2D1DeviceContext* GetDC() { return context; }

    // ANSI 문자열을 wide 문자열로 변환하는 메서드
    wstring ChangeWString(string value);

private:
    ID2D1Factory1* factory;  // Direct2D 팩토리
    // DIrectX2D API를 사용하기위한 변수. 객체생성과 초기화하는데 사용한다.

    IDWriteFactory* writeFactory;  // DirectWrite 팩토리
    // 텍스트를 렌더링하고 글꼴과 관련된 작업을 수행함.

    ID2D1Device* device;  // Direct2D 디바이스
    ID2D1DeviceContext* context;  // Direct2D 디바이스 컨텍스트

    ID2D1Bitmap1* targetBitmap;  // 렌더 타겟 비트맵
    // 그래픽 또는 텍스트를 그릴 대상이되는 이미지를 저장함

    map<string, ID2D1SolidColorBrush*> brushes;  // 색상 브러시 맵
    map<string, IDWriteTextFormat*> formats;  // 텍스트 포맷 맵

    ID2D1SolidColorBrush* curBrush;  // 현재 사용 중인 브러시
    // ID2D1SolidColorBrush = 특정 단색으로 채워진 도형을 그릴 때 사용함.
    IDWriteTextFormat* curFormat;  // 현재 사용 중인 텍스트 포맷
    // IDWriteTextFormat = 텍스트의 모양, 스타일, 정렬 등을 지정할 수 있음.
};