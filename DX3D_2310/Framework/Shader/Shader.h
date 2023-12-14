#pragma once

// 중복된 쉐이더가 있으면 원래 호출된 쉐이더를 반환하도록 코딩
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

	static unordered_map<wstring, Shader*> shaders; // 중복되는거면 있는거 반환하는 식으로
};