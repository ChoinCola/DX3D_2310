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

	if (specularMap)
		specularMap->PSSet(1);

	if (normalMap)
		normalMap->PSSet(2);

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

void Material::SetSpecularMap(wstring textureFile)
{
	specularMap = Texture::Add(textureFile);
}

void Material::SetNormalMap(wstring textureFile)
{
	normalMap = Texture::Add(textureFile);
}

void Material::DeleteDiffuseMap()
{
	diffuseMap = nullptr;
}

void Material::DeleteSpecularMap()
{
	specularMap = Texture::Add(L"Textures/Colors/White.png");
}

void Material::DeleteNormalMap()
{
	normalMap = Texture::Add(L"Textures/Colors/Blue.png");
}
