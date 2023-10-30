#pragma once

class Minimap : public Transform
{
public:
	Minimap();
	~Minimap();

	void Update();
	void GUIRender();

	void SetView();
private:
	D3D11_VIEWPORT viewPort;
	ViewBuffer* viewBuffer;
	Matrix view;
	Matrix projection;
};