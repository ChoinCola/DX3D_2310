#pragma once
/// <summary>
/// ������ ���ۻ������� CreateBuffer���� ó����.
/// </summary>
class Buffer
{
public:
	// ��������, ���ۻ�����, �㰡����, ������ ù��° �ּ�
	Buffer(UINT BindFlags, UINT ByteWidth, D3D11_USAGE Usage, void* Vector_Start);
	~Buffer();

	D3D11_INPUT_ELEMENT_DESC& GetlayoutDesc() { return *layoutDesc; }
	ID3D11Buffer** GetBuffer() { return &buffer; }
	UINT* Getstride() { return &stride; }
	UINT* Getoffset() { return &offset; }
	UINT* Getcount () { return &count; }
private:
	void BindFlags(int BindFlags);
	void SetElementDesc();
private:

	D3D11_INPUT_ELEMENT_DESC* layoutDesc = nullptr;
	ID3D11Buffer* buffer = nullptr;
	ID3DBlob* blob = nullptr;

	UINT stride = 0;
	UINT offset = 0;
	UINT count = 0;
};