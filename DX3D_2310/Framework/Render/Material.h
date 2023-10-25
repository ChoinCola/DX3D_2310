#pragma once

class Material
{
private:
	enum MapType
	{
		DIFFUSE, 
		SPECULAR,
		NORMAL
	};
public:
	Material(wstring shderFIle, wstring name = {});
	~Material();

	void GUIRneder();

	void Set();

	void SetShader(wstring shaderFile);

	void SetDiffuseMap(wstring textureFile);
	void SetSpecularMap(wstring textureFile);
	void SetNormalMap(wstring textureFile);

	void DeleteDiffuseMap();
	void DeleteSpecularMap();
	void DeleteNormalMap();

	MaterialBuffer::Data* GetBuffer() { return buffer->GetData(); }

	wstring GetName() { return name; }
	void SetName(wstring name) { this->name = name; }

private:
	void SelectMap(string name, MapType mapType);
	void UnselectMap(MapType mapType);

	void Save();
	void Load();
	void SaveTexture(Texture*& data, BinaryWriter*& writer);
	void LoadTexture(Texture* data, BinaryReader* reader, MapType num);


private:
	static UINT material_Key;
	wstring name = {};
	wstring editName;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* normalMap = nullptr;
	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;

	MaterialBuffer* buffer;
};
