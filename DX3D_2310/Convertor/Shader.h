#pragma once
/// <summary>
/// ������ ���̴��������� CreateShader���� ó����.
/// </summary>
class Shader
{
public:
	//���̴� �ּ�
	Shader(LPCWSTR input, DWORD flags);
	~Shader();

	void SetShader(LPCWSTR input, DWORD flags);
	ID3D11VertexShader* GetvertexShader() { return vertexShader; }
	ID3D11PixelShader* GetpixelShader() { return pixelShader; }
	ID3D11InputLayout* GetinputLayout() { return inputLayout; }     // ��ǲ ���̾ƿ�

private:
	ID3D11VertexShader* vertexShader;	// ���ؽ� ���̴�
	ID3D11PixelShader* pixelShader;     // �ȼ� ���̴�
	ID3D11InputLayout* inputLayout;     // ��ǲ ���̾ƿ�
	ID3DBlob* blob;						// blob

};