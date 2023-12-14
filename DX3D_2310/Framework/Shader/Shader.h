#pragma once

// �ߺ��� ���̴��� ������ ���� ȣ��� ���̴��� ��ȯ�ϵ��� �ڵ�
class Shader
{
protected:
	Shader() = default;
	virtual ~Shader();

public:
	static class VertexShader* AddVS(wstring fiile);
	static class PixelShader* AddPS(wstring fiile);
	static class ComputeShader* AddCS(wstring fiile);

	static void Delete();

	virtual void Set() = 0;

protected:
	ID3DBlob* blob = nullptr;

	static unordered_map<wstring, Shader*> shaders; // �ߺ��Ǵ°Ÿ� �ִ°� ��ȯ�ϴ� ������
};