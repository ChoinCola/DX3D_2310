#pragma once

class RawBuffer
{
public:
    RawBuffer(UINT byteWidth);
    ~RawBuffer();

    void Copy(void* data, UINT size);

    // GPU데이터를 받아오는 역할을 한다. 
    ID3D11UnorderedAccessView*& GetUAV() { return uav; }

private:
    void CreateOutput();
    void CreateUAV();
    void CreateResult();

private:
    ID3D11Buffer* output;
    ID3D11UnorderedAccessView* uav;

    ID3D11Buffer* result;

    UINT byteWidth;
};