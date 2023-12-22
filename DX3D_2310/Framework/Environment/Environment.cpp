#include "Framework.h"

Environment::Environment()
{
	CreateProjection();
	CreateState();

	SetViewport();

	mainCamera = new Camera();
	lightBuffer = new LightBuffer();
	uiViewBuffer = new ViewBuffer();

	lightBuffer->GetData()->lights[0].isActive = true;
	lightBuffer->Load("TextData/Light/", 0);
}

Environment::~Environment()
{
	lightBuffer->Save("TextData/Light/", 0);

	delete projectionBuffer;

	delete samplerState;

	delete rasterizerSate[0];
	delete rasterizerSate[1];

	delete mainCamera;
	delete lightBuffer;
}

void Environment::Update()
{
	if(KEY->Down(VK_F1))
		isWireMode = !isWireMode;

	mainCamera->Update();

	//if(Mouse::Get()->GetIsSetMouseHold())
	//	CamMove();
}

void Environment::GUIRender()
{
	mainCamera->GUIRender();
	if (ImGui::TreeNode("LightOption"))
	{
		if (ImGui::Button("Add"))
			lightBuffer->GetData()->lightCount++;
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
			lightBuffer->GetData()->lightCount--;

		FOR(lightBuffer->GetData()->lightCount) {
			if (ImGui::TreeNode(("Light_" + to_string(i)).c_str())) {
				EditLight(&lightBuffer->GetData()->lights[i], i);
				SaveDialog(i);
				LoadDialog(i);
				ImGui::TreePop();
			}
		}

		ImGui::ColorEdit3("AmbientLight", (float*)&lightBuffer->GetData()->ambientLight);
		ImGui::ColorEdit3("AmbientCeil", (float*)&lightBuffer->GetData()->ambientCeil);
		ImGui::TreePop();
	}
}

void Environment::Set()
{
	rasterizerSate[isWireMode]->SetState();
	blendState[1]->SetState();
	depthState[0]->SetState();

	mainCamera->SetView();
	lightBuffer->SetPS(0);

	projectionBuffer->Set(projection);
	projectionBuffer->SetVS(2);
}

void Environment::SetPost()
{
	blendState[1]->SetState();
	depthState[1]->SetState();

	uiViewBuffer->SetVS(1);
	projectionBuffer->Set(orthograpic);
	projectionBuffer->SetVS(2);
}

void Environment::SetViewport(UINT width, UINT height)
{
	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	DC->RSSetViewports(1, &viewPort);
}

void Environment::CreateProjection()
{

	projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
		(float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.0f);

	//	XMMaxrixOrthographicOffCenterLH() = 가운데가 0으로 설정하는걸 안하고, 왼손좌표계
	orthograpic = XMMatrixOrthographicOffCenterLH(0.0f, WIN_WIDTH,
		0.0f, WIN_HEIGHT, -1.0f, 1.0f);

	projectionBuffer = new MatrixBuffer();
}

void Environment::CreateState()
{
	samplerState = new SamplerState();
	samplerState->SetState();

	rasterizerSate[0] = new RasterizerState();
	rasterizerSate[1] = new RasterizerState();

	rasterizerSate[1]->FillMode(D3D11_FILL_WIREFRAME);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);
	//blendState[1]->AlphaToCoverage(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);
}

void Environment::EditLight(LightBuffer::Light* light, int index)
{
	string label = "Light_" + to_string(index);

	if (ImGui::TreeNode(label.c_str()))
	{
		ImGui::Checkbox("Active", (bool*)&light->isActive);
		const char* list[] = { "Directional","Point", "Spot" };
		ImGui::Combo("Type", &light->type, list, 3);

		ImGui::ColorEdit3("Color", (float*)&light->color);
		ImGui::SliderFloat3("Directoin", (float*)&light->direction, -1, 1);
		ImGui::SliderFloat3("Position", (float*)&light->position, -100, 100);
		ImGui::SliderFloat("Range", (float*)&light->range, 1, 300);
		ImGui::SliderFloat("Inner", (float*)&light->inner, 1, 180);
		ImGui::SliderFloat("Outer", (float*)&light->outer, 1, 180);
		ImGui::SliderFloat("AttentionIntensity", (float*)&light->attentionIntensity, 0.1f, 3.0f);

		ImGui::TreePop();
	}
}

// 우측으로 갈 때 z는 sin만큼 떨어지고, x는 cos만큼 올라야함.
// 좌측으로 갈 때 z는 sin만큼 떨어지고, x는 cos만큼 떨어져야함.

void Environment::SaveDialog(UINT i)
{
	string key = "Save";

	if (ImGui::Button(key.c_str()))
		lightBuffer->Save("TextData/Light/", i);

	ImGui::SameLine();

	key = "SaveAs";

	if (ImGui::Button(key.c_str()))
		DIALOG->OpenDialog(key.c_str(), key.c_str(), ".lu", ".");

	if (DIALOG->Display(key.c_str()))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			lightBuffer->Save(file, i);
		}

		DIALOG->Close();
	}
}

void Environment::LoadDialog(UINT i)
{
	string key = "Load";

	if (ImGui::Button(key.c_str()))
		DIALOG->OpenDialog(key.c_str(), key.c_str(), ".lu", ".");

	if (DIALOG->Display(key.c_str()))
	{
		if (DIALOG->IsOk())
		{
			string file = DIALOG->GetFilePathName();

			lightBuffer->Load(file, i);
		}

		DIALOG->Close();
	}
}
