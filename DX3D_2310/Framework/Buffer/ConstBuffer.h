#pragma once
class ConstBuffer
{
public:
	ConstBuffer(void* data, UINT dataSize);
	~ConstBuffer();

	void SetVS(UINT slot);
	void SetPS(UINT slot);

private:
	ID3D11Buffer* buffer = nullptr;

	void* data;
	UINT dataSize;
};