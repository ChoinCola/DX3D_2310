#pragma once

class Material
{
public:
	Material(wstring shderFIle);
	~Material();

	void Set();

	void SetShader(wstring shaderFile);

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
};