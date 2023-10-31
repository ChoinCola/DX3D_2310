#pragma once

class GameObject : public Transform
{
public:
	GameObject(wstring shaderfile = L"Light/Light.hlsl");
	~GameObject();

	void SetRender();

	Material* GetMaterial() { return material; }

protected:
	Material* material;
	MatrixBuffer* worldBuffer;
};