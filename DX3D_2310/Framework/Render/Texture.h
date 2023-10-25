#pragma once

class Texture
{
private:
	// ��������ڰ� �ƴ϶� �״�� ������ �����ڷ� �̵����� ����Ѵ�
	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file);
	~Texture();

public:
	void PSSet(UINT slot = 0);
	void ReadPixels(vector<Float4>& pixels);

	Float2 GetSize() {
		return Float2(image.GetMetadata().width, image.GetMetadata().height);
	}

	ID3D11ShaderResourceView* GetSRV() { return srv; }

public:
	static Texture* Add(wstring file);
	static Texture* Add(wstring file, wstring key);

	static void Delete();

private:
	wstring file;

	ScratchImage image;

	ID3D11ShaderResourceView* srv;

	static unordered_map<wstring, Texture*> textures;
};