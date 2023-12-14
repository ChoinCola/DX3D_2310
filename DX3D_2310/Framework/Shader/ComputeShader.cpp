#include "Framework.h"

ComputeShader::ComputeShader(wstring file)
{
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    //D3D_COMPILE_STANDARD_FILE_INCLUDE 쉐이더 로드용 헤더를 쓴다는뜻
    HRESULT result = D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "CS", "cs_5_0", flags, 0, &blob, nullptr);

    assert(SUCCEEDED(result));

    DEVICE->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), 
        nullptr, &computeShader);
}

ComputeShader::~ComputeShader()
{
    computeShader->Release();
}

void ComputeShader::Set()
{
    DC->CSSetShader(computeShader, nullptr, 0);
}
