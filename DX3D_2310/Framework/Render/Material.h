#pragma once

class Material
{
public:
	Material(wstring shderFIle);
	~Material();

	void Set();

	void SetShader(wstring shaderFile);

	void SetDiffuseMap(wstring textureFile);
	void SetSpecularMap(wstring textureFile);
	void SetNormalMap(wstring textureFile);

	void DeleteDiffuseMap();
	void DeleteSpecularMap();
	void DeleteNormalMap();


private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* normalMap = nullptr;
	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
};