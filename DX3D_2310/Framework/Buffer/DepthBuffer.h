#pragma once
class DepthBuffer : public Singleton<DepthBuffer>
{
private:
	friend class Singleton;

	DepthBuffer();
	~DepthBuffer();

public:
	void SetPS(UINT slot);

private:
	ID3D11Texture2D* DepthStencil = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;
};