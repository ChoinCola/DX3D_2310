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
	if (diffuseMap)
		diffuseMap->PSSet(0);
	vertexShader->Set();
	pixelShader->Set();
}

void Material::SetShader(wstring shaderFile)
{
	vertexShader = Shader::AddVS(shaderFile);
	pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetDiffuseMap(wstring textureFile)
{
	diffuseMap = Texture::Add(textureFile);
}
