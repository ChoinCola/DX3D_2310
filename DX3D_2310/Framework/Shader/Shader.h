#pragma once

// �ߺ��� ���̴��� ������ ���� ȣ��� ���̴��� ��ȯ�ϵ��� �ڵ�
class Shader
{
protected:
	Shader() = default;
	virtual ~Shader();

public:
	static class VertexShader* AddVS(wstring fiile, int flags);
	static class PixelShader* AddPS(wstring fiile);

	static void Delete();

	virtual void Set() = 0;

protected:
	ID3DBlob* blob;

	static unordered_map<wstring, Shader*> shaders; // �ߺ��Ǵ°Ÿ� �ִ°� ��ȯ�ϴ� ������
};