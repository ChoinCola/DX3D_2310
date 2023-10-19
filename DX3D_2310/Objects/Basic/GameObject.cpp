#include "Framework.h"

GameObject::GameObject(wstring shaderfile)
{
	material = new Material(shaderfile);
	worldBuffer = new MatrixBuffer();
}

GameObject::~GameObject()
{
	delete material;
	delete worldBuffer;
}

void GameObject::SetRender()
{
	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	material->Set();
}