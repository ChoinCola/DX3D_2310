#pragma once

class VertexShader : public Shader
{
private:
	friend class Shader;

	VertexShader(wstring file);
	~VertexShader();

public:
	virtual void Set() override;

private:
	void CreateInputLayout();

private:
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

	ID3D11ShaderReflection* reflection;
	// 버텍스 쉐이더에 들어가서 여러가지 데이터를 빼옴.
};