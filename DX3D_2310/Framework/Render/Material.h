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
	Material(wstring shderFIle = L"Light/Light.hlsl");
	~Material();

	void GUIRender();

	void Set();

	void SetShader(wstring shaderFile);

	void SetDiffuseMap(wstring textureFile);
	void SetDiffuseMap(Texture* textureFile) { diffuseMap = textureFile; };
	Texture* GetDiffuseMap() { return diffuseMap; };

	void SetSpecularMap(wstring textureFile);
	void SetNormalMap(wstring textureFile);

	void Save(string file);
	void Load(string file);

	MaterialBuffer::Data* GetBuffer() { return buffer->GetData(); }

	string GetName() { return name; }
	void SetName(string name) { this->name = name; }

private:
	void SelectShader();
	void SelectMap(string name, MapType mapType);
	void UnselectMap(MapType mapType);

	void SaveDialog();
	void LoadDialog();

private:
	static UINT material_Key;
	string name = {};
	string editName;
	string file;
	string shaderFile;
	string projectPath;
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* normalMap = nullptr;
	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;

	MaterialBuffer* buffer;
};
