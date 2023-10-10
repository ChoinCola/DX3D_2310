#pragma once

// 디바이스 크리에이트
class Device : public Singleton<Device>
{
private:
	// 나 스스로와 싱글톤 에서만 접근이 가능하다.
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

	IDXGISwapChain* swapChain; // 백버퍼 관리 인터페이스
	ID3D11RenderTargetView* renderTargetView; // GPU쪽 관련 메모리를 접근하는 용도. 중재역활
};