#pragma once

// 깊이 스텐실 상태를 관리하는 클래스입니다.
class DepthStencilState
{
public:
    DepthStencilState();
    ~DepthStencilState();

    // 현재 깊이 스텐실 상태를 설정합니다.
    void SetState();

    // 깊이 테스팅을 활성화 또는 비활성화합니다.
    void DepthEnable(bool value);

    // 깊이 버퍼에 대한 쓰기 마스크를 설정합니다.
    void DepthWriteMask(D3D11_DEPTH_WRITE_MASK value);

    // 깊이 스텐실 상태가 변경되었음을 표시합니다.
    void Changed();

private:
    // D3D11_DEPTH_STENCIL_DESC 구조체로 정의된 깊이 스텐실 상태를 저장하는 멤버 변수입니다.
    D3D11_DEPTH_STENCIL_DESC desc = {};

    // 현재 설정된 깊이 스텐실 상태를 나타내는 인터페이스 멤버 변수입니다.
    ID3D11DepthStencilState* state = nullptr;
};