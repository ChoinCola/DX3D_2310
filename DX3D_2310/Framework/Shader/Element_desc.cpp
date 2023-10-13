#include "Framework.h"

Element_desc::Element_desc()
{
	if (desc.empty()) {
		vector<D3D11_INPUT_ELEMENT_DESC> layoutDescColor =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
				D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		desc.emplace_back(layoutDescColor);

		vector<D3D11_INPUT_ELEMENT_DESC> layoutDescUV =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
				D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		desc.emplace_back(layoutDescUV);
	}
}

Element_desc::~Element_desc()
{
}