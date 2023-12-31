#pragma once

class Texture
{
private:
	// 복사생성자가 아니라 그대로 참조형 생성자로 이동시켜 사용한다
	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file);
	~Texture();

public:
	void PSSet(UINT slot = 0);
	void ReadPixels(vector<Float4>& pixels);

	Float2 GetSize() {
		return Float2(image.GetMetadata().width, image.GetMetadata().height);
	}
	wstring& GetFile() { return file; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

public:
	static Texture* Add(wstring file);
	static Texture* Add(wstring file, wstring key);
	static Texture* Add(wstring key, ID3D11ShaderResourceView* srv);

	static void Delete();

private:
	wstring file;
	bool isReferenced = false; // 참조여부검사

	ScratchImage image;

	ID3D11ShaderResourceView* srv;

	static unordered_map<wstring, Texture*> textures;
};