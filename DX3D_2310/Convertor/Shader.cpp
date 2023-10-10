#include "Framework.h"
#include "Shader.h"

Shader::Shader(LPCWSTR input, DWORD flags)
{

    D3DCompileFromFile(input, nullptr, nullptr, "VS", "vs_5_0", flags, 0, &blob, nullptr);
    DEVICE->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D11_INPUT_PER_VERTEX_DATA, 0},
        // 포지션 인풋을 잡아준다.
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,// 앞에 몃바이트를 썼나? 표시
        D3D11_INPUT_PER_VERTEX_DATA, 0} // 컬러 인풋을 잡아준다
    };  // 인스턴싱할때 바꾼다.

    UINT layoutSize = ARRAYSIZE(layoutDesc);

    DEVICE->CreateInputLayout(layoutDesc, layoutSize,
        blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);

    blob->Release();
    D3DCompileFromFile(input, nullptr, nullptr, "PS", "ps_5_0", flags, 0, &blob, nullptr);
    DEVICE->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
}

Shader::~Shader()
{
}

void Shader::SetShader(LPCWSTR input, DWORD flags)
{
}
