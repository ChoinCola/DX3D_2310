#pragma once

class Material
{
public:
	Material(wstring shderFIle, int flag);
	~Material();

	void Set();

	void SetShader(wstring shaderFile, int flag);

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
};