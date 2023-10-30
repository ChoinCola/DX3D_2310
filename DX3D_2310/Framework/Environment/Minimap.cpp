#include "Framework.h"
#include "Minimap.h"

Minimap::Minimap()
{
	tag = "Minimap";
	viewBuffer = new ViewBuffer();

	viewPort.TopLeftX = WIN_WIDTH * 0.8f; // 화면의 오른쪽 하단에 위치
	viewPort.TopLeftY = WIN_HEIGHT * 0.8f;
	viewPort.Width = WIN_WIDTH * 0.2f; // 화면 크기의 20%로 설정
	viewPort.Height = WIN_HEIGHT * 0.2f;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	viewBuffer->SetVS(1);
	viewBuffer->SetPS(1);
}

Minimap::~Minimap()
{
	delete viewBuffer;
}

void Minimap::Update()
{
	projection = Environment::Get()->GetProjection();
	UpdateWorld();
}

void Minimap::GUIRender()
{
	if (ImGui::TreeNode("CameraOption"))
	{
		ImGui::DragFloat("TopLeftY", &viewPort.TopLeftY);
		ImGui::DragFloat("TopLeftX", &viewPort.TopLeftX);

		ImGui::TreePop();
	}
}

void Minimap::SetView()
{
	DC->RSSetViewports(1, &viewPort);
	view = XMMatrixInverse(nullptr, world);
	viewBuffer->Set(view, world);

	viewBuffer->SetVS(1);
	viewBuffer->SetPS(1);
}