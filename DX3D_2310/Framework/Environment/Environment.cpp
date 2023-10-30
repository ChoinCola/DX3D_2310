#include "Framework.h"

Environment::Environment()
{
	CreateProjection();
	CreateState();

	SetViewport();
	minimap = new Minimap();
	mainCamera = new Camera();
	lightBuffer = new LightBuffer();
	uiViewBuffer = new ViewBuffer();
}

Environment::~Environment()
{
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
	minimap->Translate(mainCamera->GetLocalPosition() + Vector3(0, 100, 0));
	minimap->Rotate(Vector3(90, 0, 0));
	minimap->Update();
	//if(Mouse::Get()->GetIsSetMouseHold())
	//	CamMove();
}

void Environment::GUIRender()
{
	mainCamera->GUIRender();

	ImGui::SliderFloat3("LightDirectoin", (float*)&lightBuffer->GetData()->lightDirection, -1, 1);
	ImGui::ColorEdit3("AmbientLight", (float*)&lightBuffer->GetData()->ambientLight);
	ImGui::Checkbox("MinimapSet", &IsPrintMinimap);
	minimap->GUIRender();
}

void Environment::Set()
{
	rasterizerSate[isWireMode]->SetState();
	blendState[0]->SetState();

	SetViewport();

	mainCamera->SetView();
	lightBuffer->SetPS(0);

	projectionBuffer->Set(projection);
	projectionBuffer->SetVS(2);
}

void Environment::SetPost()
{
	blendState[1]->SetState();

	uiViewBuffer->SetVS(1);
	projectionBuffer->Set(orthograpic);
	projectionBuffer->SetVS(2);
}

void Environment::SetMinimap()
{
	rasterizerSate[isWireMode]->SetState();
	blendState[0]->SetState();


	minimap->SetView();
	lightBuffer->SetPS(0);

	projectionBuffer->Set(projection);
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

	//	XMMaxrixOrthographicOffCenterLH() = ����� 0���� �����ϴ°� ���ϰ�, �޼���ǥ��
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
}

// �������� �� �� z�� sin��ŭ ��������, x�� cos��ŭ �ö����.
// �������� �� �� z�� sin��ŭ ��������, x�� cos��ŭ ����������.