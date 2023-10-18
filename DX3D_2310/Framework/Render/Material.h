#pragma once

class Material
{
public:
	Material(wstring shderFIle);
	~Material();

	void Set();

	void SetShader(wstring shaderFile);

	void SetDiffuseMap(wstring textureFile);

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* diffuseMap = nullptr;
};