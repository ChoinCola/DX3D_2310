#pragma once
/// <summary>
/// 생성시 버퍼생성부터 CreateBuffer까지 처리함.
/// </summary>
class Buffer
{
public:
	// 버퍼형태, 버퍼사이즈, 허가권한, 벡터의 첫번째 주소
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