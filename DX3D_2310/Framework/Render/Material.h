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
	Material(wstring shderFIle, string name = {});
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

	string GetName() { return name; }
	void SetName(string name) { this->name = name; }

private:
	void SelectMap(string name, MapType mapType);
	void UnselectMap(MapType mapType);

	void Save();
	void Load(string reader = "");
	void SaveTexture(Texture*& data, BinaryWriter*& writer);
	void const LoadTexture ( Texture*& data,  BinaryReader* reader,  MapType num);


private:
	static UINT material_Key;
	string name = {};
	string editName;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* normalMap = nullptr;
	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;

	MaterialBuffer* buffer;
};
