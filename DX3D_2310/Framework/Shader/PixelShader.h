#pragma once

class PixelShader
{
public:
	PixelShader(wstring file);
	~PixelShader();
	void Set();

private:
	ID3DBlob* blob;

	ID3D11PixelShader* pixelShader;
};