#include "Framework.h"

Material::Material(wstring shderFIle, int flag)
{
	SetShader(shderFIle, flag);
}

Material::~Material()
{
}

void Material::Set()
{
	vertexShader->Set();
	pixelShader->Set();
}

void Material::SetShader(wstring shaderFile, int flag)
{
	vertexShader = Shader::AddVS(shaderFile, flag);
	pixelShader = Shader::AddPS(shaderFile);
}
