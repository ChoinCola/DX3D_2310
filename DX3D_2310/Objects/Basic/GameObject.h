#pragma once

class GameObject : public Transform
{
public:
	GameObject(wstring shaderfile = L"Basic/Texture.hlsl");
	~GameObject();

	void SetRender();

	Material* GetMetrial() { return material; }

protected:
	Material* material;
	MatrixBuffer* worldBuffer;
};