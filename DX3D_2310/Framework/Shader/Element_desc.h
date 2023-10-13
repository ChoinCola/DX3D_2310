#pragma once

enum ShaderChois
{
	VERTEX_COLOR,
	VERTEX_UV
};

class Element_desc : public Singleton<Element_desc>
{
public:
	friend class Singleton;
	Element_desc();
	~Element_desc();

	vector<D3D11_INPUT_ELEMENT_DESC>& GetDESC(const int ShaderChois) { return desc[ShaderChois]; }

private:
	vector<vector<D3D11_INPUT_ELEMENT_DESC>> desc;
};