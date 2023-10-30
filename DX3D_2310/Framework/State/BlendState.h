#pragma once

class BlendState
{
public:
    BlendState();
    ~BlendState();

    void SetState();

    void Alpha(bool value);
    void Additive();
    void AlphaToCoverage(bool value);

    void Changed();

private:
    // 블렌딩 사전설정용 desc
    D3D11_BLEND_DESC desc = {};
    // 데이터 넘기기위한 버퍼
    ID3D11BlendState* state = nullptr;
};