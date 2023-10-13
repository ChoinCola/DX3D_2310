#pragma once

class VertexShader : public Shader
{
private:
	friend class Shader;

	VertexShader(wstring file, int flag = 0);
	~VertexShader();

public:
	virtual void Set() override;

private:
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;
};