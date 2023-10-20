#include "Framework.h"

Environment::Environment()
{
	CreateProjection();
	CreateState();

	SetViewport();
	mainCamera = new Camera();
	lightBuffer = new LightBuffer();
}

Environment::~Environment()
{
	delete projectionBuffer;

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

	ImGui::SliderFloat3("LightDirection", (float*)&lisghtBuffer,)
}

void Environment::Set()
{
	rasterizerSate[isWireMode]->SetState();

	mainCamera->SetView();
	lightBuffer->SetVS(10);
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

void Environment::SetPerspective()
{
}

void Environment::CreateProjection()
{

	Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
		(float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.0f);

	projectionBuffer = new MatrixBuffer();

	projectionBuffer->Set(projection);
	projectionBuffer->SetVS(2);
}

void Environment::CreateState()
{
	samplerState = new SamplerState();
	samplerState->SetState();

	rasterizerSate[0] = new RasterizerState();
	rasterizerSate[1] = new RasterizerState();

	rasterizerSate[1]->FillMode(D3D11_FILL_WIREFRAME);
}

//void Environment::CamMove()
//{
//	mousemovevalue = Mouse::Get()->GetMoveValue() * DELTA;
//	// ���콺 ȸ���Լ�. ���� ���콺�� value�� ����. focus�� ��������� ����. ������ ũ�Ⱑ ���簢���� �ƴ����� ����
//	focus += { +mousemovevalue.x, -mousemovevalue.y * WIN_HEIGHT / WIN_WIDTH, -mousemovevalue.x, 0 };
//	if (XMVectorGetX(focus) >= 360) XMVectorSetX(focus, 0);
//	if (XMVectorGetZ(focus) <= -360) XMVectorSetZ(focus, 0);
//
//	// ���� �ٶ󺸴� ������ Position���� �������� �ѷ����Ѵ�.
//	Vector3 focusnow =
//	{ XMVectorGetX(eye) + sin(XMVectorGetX(focus)),
//		XMVectorGetY(eye) + sin(XMVectorGetY(focus)),
//		XMVectorGetZ(eye) - sin(XMVectorGetZ(focus)) };
//	// ���� ���������� ���⸸ ����
//	Vector3 focusnowUp = { XMVectorGetX(eye), XMVectorGetY(eye) + sin(XMVectorGetY(focus)), XMVectorGetZ(eye) };
//
//
//	// �̵������� ���밪 ������ �ƴ� �����ǥ �������� �������� ������. focusnow�� ���� pos�� Nomalize���� �ʿ���.
//	Vector3 front = XMVector3Normalize(focusnow - eye);	// ����
//	// ������� ������� ��ǥ���� y�� 0���� �ʱ�ȭ
//	front.y = 0.0f;
//	// ����������� Y���� 90�� ȸ����Ų��. ���� �����ǥ�� ����
//	Vector3 focusnowLeft = eye + XMVector3TransformCoord(front, XMMatrixRotationY(-XM_PIDIV2));
//
//	if (KEY->Press('W')) eye += XMVector3Normalize(focusnow		- eye) * CamSpeed * DELTA;
//	if (KEY->Press('S')) eye -= XMVector3Normalize(focusnow		- eye) * CamSpeed * DELTA;
//	if (KEY->Press('A')) eye += XMVector3Normalize(focusnowLeft - eye) * CamSpeed * DELTA;
//	if (KEY->Press('D')) eye -= XMVector3Normalize(focusnowLeft - eye) * CamSpeed * DELTA;
//	if (KEY->Press('E')) eye += XMVector3Normalize(focusnowUp	- eye) * CamSpeed * DELTA;
//	if (KEY->Press('Q')) eye -= XMVector3Normalize(focusnowUp	- eye) * CamSpeed * DELTA;
//
//	// �̵��Ŀ� �ٽ��ѹ� ��ǥ�� ����Ѵ�.
//	focusnow = {
//		XMVectorGetX(eye) + sin(XMVectorGetX(focus)) * 0.2f,
//		XMVectorGetY(eye) + sin(XMVectorGetY(focus)),
//		XMVectorGetZ(eye) - sin(XMVectorGetZ(focus)) * 0.2f };
//
//	Matrix view = XMMatrixLookAtLH(eye, focusnow, up);
//	viewBuffer->Set(view);
//	viewBuffer->SetVS(1);
//}
//

// �������� �� �� z�� sin��ŭ ��������, x�� cos��ŭ �ö����.
// �������� �� �� z�� sin��ŭ ��������, x�� cos��ŭ ����������.