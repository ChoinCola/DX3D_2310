#pragma once
/// <summary>
/// »ý¼º½Ã ¼ÎÀÌ´õ»ý¼ººÎÅÍ CreateShader±îÁö Ã³¸®ÇÔ.
/// </summary>
class Shader
{
public:
	//½¦ÀÌ´õ ÁÖ¼Ò
	Shader(LPCWSTR input, DWORD flags);
	~Shader();

	void SetShader(LPCWSTR input, DWORD flags);
	ID3D11VertexShader* GetvertexShader() { return vertexShader; }
	ID3D11PixelShader* GetpixelShader() { return pixelShader; }
	ID3D11InputLayout* GetinputLayout() { return inputLayout; }     // ÀÎÇ² ·¹ÀÌ¾Æ¿ô

private:
	ID3D11VertexShader* vertexShader;	// ¹öÅØ½º ½¦ÀÌ´õ
	ID3D11PixelShader* pixelShader;     // ÇÈ¼¿ ½¦ÀÌ´õ
	ID3D11InputLayout* inputLayout;     // ÀÎÇ² ·¹ÀÌ¾Æ¿ô
	ID3DBlob* blob;						// blob

};