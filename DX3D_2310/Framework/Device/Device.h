#pragma once

// ����̽� ũ������Ʈ
class Device : public Singleton<Device>
{
private:
	// �� �����ο� �̱��� ������ ������ �����ϴ�.
	friend class Singleton;

	Device();
	~Device();
public:
	void Clear();
	void Present();
	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDC() { return deviceContext; }
	
private:
	void CreateDeviceAndSwapChain();

private:
	ID3D11Device* device;   // CPU
	ID3D11DeviceContext* deviceContext; // GPU

	IDXGISwapChain* swapChain; // ����� ���� �������̽�
	ID3D11RenderTargetView* renderTargetView; // GPU�� ���� �޸𸮸� �����ϴ� �뵵. ���翪Ȱ
};