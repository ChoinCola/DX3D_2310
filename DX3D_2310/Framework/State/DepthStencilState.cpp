#include "Framework.h"

// 생성자: 초기 깊이 스텐실 상태를 설정합니다.
DepthStencilState::DepthStencilState()
{
    desc.DepthEnable = true;  // 깊이 테스팅을 활성화합니다.
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  // 모든 깊이 값을 쓰도록 설정합니다.
    desc.DepthFunc = D3D11_COMPARISON_LESS;  // 깊이 테스트의 비교 함수를 설정합니다.

    // 스텐실 상태를 설정합니다.
    desc.StencilEnable = true;
    desc.StencilReadMask = 0xff;
    desc.StencilWriteMask = 0xff;

    // 렌더순서기준 정면
 
    // 앞면 및 뒷면의 스텐실 오퍼레이션 및 비교 함수를 설정합니다.
    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    // 스텐실 테스트가 실패했을 때 의 처리 = 실패시 현재 스텐실 유지
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    // 스텐실 테스트는 통과했지만, 깊이테스트에 실패한경우 = 스텐실값을 증가시킴
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    // 스텐실 테스트가 성공한경우 = 스텐실값을 유지
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    // 스텐실 테스트에 사용되는 비교함수 = 항상성공함.

    // 렌더순서기준 뒷면
    desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // 깊이 스텐실 상태가 변경되었음을 표시합니다.
    Changed();
}

// 소멸자: 할당된 리소스를 해제합니다.
DepthStencilState::~DepthStencilState()
{
    state->Release();
}

// 현재 깊이 스텐실 상태를 설정합니다.
void DepthStencilState::SetState()
{
    // 2DObject출력기준 최대깊이로 설정해버린다.
    // 이럼으로써 모든 객체를 동일한 위치에서 계산할 수 있게된다.
    DC->OMSetDepthStencilState(state, 1);
}

// 깊이 테스팅을 활성화 또는 비활성화합니다.
void DepthStencilState::DepthEnable(bool value)
{
    // 깊이스텐실을 사용할것 인지 아닌지 설정함.
    desc.DepthEnable = value;  // 깊이버퍼를 켜거나 끄도록 설정합니다.

    // 깊이 스텐실 상태가 변경되었음을 표시합니다.
    Changed();
}

// 깊이 버퍼에 대한 쓰기 마스크를 설정합니다.
void DepthStencilState::DepthWriteMask(D3D11_DEPTH_WRITE_MASK value)
{
    /*
    D3D11_DEPTH_WRITE_MASK_ZERO: 
    깊이 버퍼에 아무것도 쓰지 않음. 쓰기가 비활성화되며, 
    깊이 정보가 업데이트되지 않습니다.

    D3D11_DEPTH_WRITE_MASK_ALL: 
    모든 픽셀의 깊이 정보를 깊이 버퍼에 쓰는 것을 허용함. 
    깊이 정보가 업데이트됩니다.
    */
    desc.DepthWriteMask = value;

    // 깊이 스텐실 상태가 변경되었음을 표시합니다.
    Changed();
}

// 깊이 스텐실 상태가 변경되었을 때 호출되며, 변경된 상태를 적용합니다.
void DepthStencilState::Changed()
{
    if (state != nullptr)
        state->Release();  // 이전 상태를 해제합니다.

    // 새로운 깊이 스텐실 상태를 생성하여 설정합니다.
    DEVICE->CreateDepthStencilState(&desc, &state);
}
