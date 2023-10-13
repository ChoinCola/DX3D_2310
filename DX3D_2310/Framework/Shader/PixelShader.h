#pragma once

class PixelShader : Shader
{
private:
	friend class Shader;

	PixelShader(wstring file);
	~PixelShader();

public:
	virtual void Set() override;

private:
	ID3D11PixelShader* pixelShader;
};