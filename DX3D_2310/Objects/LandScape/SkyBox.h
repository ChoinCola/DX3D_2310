#pragma once

class SkyBox : public Sphere
{
public:
	SkyBox(wstring textureFile);
	~SkyBox();

	void Render();

private:
	Texture* cubeMap;

	RasterizerState* rasterizerSate[2];
	DepthStencilState* depthState[2];
};