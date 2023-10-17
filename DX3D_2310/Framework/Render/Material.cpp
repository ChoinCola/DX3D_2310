#include "Framework.h"

Material::Material(wstring shderFIle)
{
	SetShader(shderFIle);
}

Material::~Material()
{
}

void Material::Set()
{
	vertexShader->Set();
	pixelShader->Set();
}

void Material::SetShader(wstring shaderFile)
{
	vertexShader = Shader::AddVS(shaderFile);
	pixelShader = Shader::AddPS(shaderFile);
}
