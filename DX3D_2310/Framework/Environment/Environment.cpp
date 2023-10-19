#include "Framework.h"

Environment::Environment()
{
	CreateProjection();
	CreateState();

	SetViewport();
}

Environment::~Environment()
{
	delete viewBuffer;
	delete projectionBuffer;

	delete rasterizerSate[0];
	delete rasterizerSate[1];

}

void Environment::Update()
{
	if(KEY->Down(VK_F1))
		isWireMode = !isWireMode;

	if(Mouse::Get()->GetIsSetMouseHold())
		CamMove();
}

void Environment::Set()
{
	rasterizerSate[isWireMode]->SetState();
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
	eye = XMVectorSet(3, 50, -50, 0);//Cam Pos
	focus = XMVectorSet(100, 0, 100, 0);//Cam Look at Pos
	up = XMVectorSet(0, 1, 0, 0);//Cam Up Vector

	Matrix view = XMMatrixLookAtLH(eye, focus, up);

	Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
		(float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.0f);

	viewBuffer = new MatrixBuffer();
	projectionBuffer = new MatrixBuffer();

	viewBuffer->Set(view);
	viewBuffer->SetVS(1);

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

void Environment::CamMove()
{
	mousemovevalue = Mouse::Get()->GetMoveValue() * 0.001;

	focus += { mousemovevalue.x, -mousemovevalue.y * 0.1f, -mousemovevalue.x, 0 };
	// ���� �ٶ󺸴� ������ Position���� �������� �ѷ����Ѵ�.
	Vector3 focusnow = { XMVectorGetX(eye) + sin(XMVectorGetX(focus)) * 0.1f, XMVectorGetY(eye) + sin(XMVectorGetY(focus)), XMVectorGetZ(eye) - sin(XMVectorGetZ(focus)) * 0.1f };
	Vector3 focusnowUp = { XMVectorGetX(eye), XMVectorGetY(eye) + sin(XMVectorGetY(focus)), XMVectorGetZ(eye) };


	// �̵������� ���밪 ������ �ƴ� �����ǥ �������� �������� ������. focusnow�� ���� pos�� Nomalize���� �ʿ���.
	Vector3 front =	XMVector3Normalize(focusnow	- eye);	// ����
	// ������� ������� ��ǥ���� y�� 0���� �ʱ�ȭ
	front.y = 0.0f;
	// ����������� Y���� 90�� ȸ����Ų��. ���� �����ǥ�� ����
	Vector3 focusnowLeft = eye + XMVector3TransformCoord(front, XMMatrixRotationY(-XM_PIDIV2)); 

	if (KEY->Press('W')) eye += XMVector3Normalize(focusnow		- eye) * CamSpeed;
	if (KEY->Press('S')) eye -= XMVector3Normalize(focusnow		- eye) * CamSpeed;
	if (KEY->Press('A')) eye += XMVector3Normalize(focusnowLeft - eye) * CamSpeed;
	if (KEY->Press('D')) eye -= XMVector3Normalize(focusnowLeft - eye) * CamSpeed;
	if (KEY->Press('E')) eye += XMVector3Normalize(focusnowUp	- eye) * CamSpeed;
	if (KEY->Press('Q')) eye -= XMVector3Normalize(focusnowUp	- eye) * CamSpeed;

	focusnow = { XMVectorGetX(eye) + sin(XMVectorGetX(focus)) * 0.1f, XMVectorGetY(eye) + sin(XMVectorGetY(focus)), XMVectorGetZ(eye) - sin(XMVectorGetZ(focus)) * 0.1f };
	Matrix view = XMMatrixLookAtLH(eye, focusnow, up);
	viewBuffer->Set(view);
	viewBuffer->SetVS(1);
}
