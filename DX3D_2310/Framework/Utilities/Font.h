#pragma once

class Font : public Singleton<Font>
{
public:
    Font();
    ~Font();

    // �ؽ�Ʈ�� ������ �߰��ϴ� �޼���. ���̴�ó�� ���˴ϴ�.
    void AddColor(string key, float r, float g, float b);

    // �ؽ�Ʈ ��Ÿ���� �߰��ϴ� �޼���. �⺻�� ���� ����.
    void AddStyle(string key, wstring font, float size = 20.0f,
        DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, // ������
        DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL,// ��Ʈ ����̱��
        DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL); // ���� �����

    // ������ Ű�� �ش��ϴ� ������ �����մϴ�.
    void SetColor(string key);

    // ������ Ű�� �ش��ϴ� ��Ÿ���� �����մϴ�.
    void SetStyle(string key);

    // �ؽ�Ʈ�� �������ϴ� �޼���. ��ġ�� ũ�� ���� ����.
    void RenderText(wstring text, Float2 pos, Float2 size = Float2());
    void RenderText(string text, Float2 pos, Float2 size = Float2());

    // ������ ����̽� ���ؽ�Ʈ�� ��ȯ�մϴ�.
    ID2D1DeviceContext* GetDC() { return context; }

    // ANSI ���ڿ��� wide ���ڿ��� ��ȯ�ϴ� �޼���
    wstring ChangeWString(string value);

private:
    ID2D1Factory1* factory;  // Direct2D ���丮
    // DIrectX2D API�� ����ϱ����� ����. ��ü������ �ʱ�ȭ�ϴµ� ����Ѵ�.

    IDWriteFactory* writeFactory;  // DirectWrite ���丮
    // �ؽ�Ʈ�� �������ϰ� �۲ð� ���õ� �۾��� ������.

    ID2D1Device* device;  // Direct2D ����̽�
    ID2D1DeviceContext* context;  // Direct2D ����̽� ���ؽ�Ʈ

    ID2D1Bitmap1* targetBitmap;  // ���� Ÿ�� ��Ʈ��
    // �׷��� �Ǵ� �ؽ�Ʈ�� �׸� ����̵Ǵ� �̹����� ������

    map<string, ID2D1SolidColorBrush*> brushes;  // ���� �귯�� ��
    map<string, IDWriteTextFormat*> formats;  // �ؽ�Ʈ ���� ��

    ID2D1SolidColorBrush* curBrush;  // ���� ��� ���� �귯��
    // ID2D1SolidColorBrush = Ư�� �ܻ����� ä���� ������ �׸� �� �����.
    IDWriteTextFormat* curFormat;  // ���� ��� ���� �ؽ�Ʈ ����
    // IDWriteTextFormat = �ؽ�Ʈ�� ���, ��Ÿ��, ���� ���� ������ �� ����.
};