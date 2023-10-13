#include "Framework.h"

VertexShader::VertexShader(wstring file, int flag)
{
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    D3DCompileFromFile(file.c_str(), nullptr, nullptr,
        "VS", "vs_5_0", flags, 0, &blob, nullptr);

    DEVICE->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(),
        nullptr, &vertexShader);

    ELE_DESC->GetDESC(flag);
    UINT layoutSize = ELE_DESC->GetDESC(flag).size();

    DEVICE->CreateInputLayout(ELE_DESC->GetDESC(flag).data(), layoutSize,
        blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
}

VertexShader::~VertexShader()
{
    blob->Release();
    vertexShader->Release();
}

void VertexShader::Set()
{
    DC->IASetInputLayout(inputLayout);
    DC->VSSetShader(vertexShader, nullptr, 0);
}
