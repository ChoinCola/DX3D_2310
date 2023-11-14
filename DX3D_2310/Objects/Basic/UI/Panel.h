#pragma once

class Panel : public Quad
{
public:
	Panel(wstring texturefile);
	~Panel();

	void Update();
	void GUIRender();
protected:
	Transform* startIcon;

private:
	Vector3 dragOffset;
	bool isDrag = false;
};