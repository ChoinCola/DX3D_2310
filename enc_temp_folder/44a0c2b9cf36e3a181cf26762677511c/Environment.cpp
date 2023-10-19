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
	eye =	XMVectorSet(0, 0, 0, 0);//Cam Pos
	focus = XMVectorSet(0, 0, -1, 0);//Cam Look at Pos
	up =	XMVectorSet(0, 1, 0, 0);//Cam Up Vector

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
	mousemovevalue = Mouse::Get()->GetMoveValue() * DELTA * 50;
	// ���콺 ȸ���Լ�. ���콺 value�� ���� ī�޶��� ����Ʈ ����.

	// ���� �ٶ󺸴� ������ Position���� �������� �ѷ����Ѵ�.
	// Pitch (X �� ���� ȸ��)

	XMMATRIX roY = XMMatrixRotationX(XMConvertToRadians(-mousemovevalue.y));
	XMMATRIX roX = XMMatrixRotationY(XMConvertToRadians(+mousemovevalue.x));
	XMMATRIX roZ = XMMatrixRotationZ(XMConvertToRadians(+mousemovevalue.x));

	//focus = XMVector3TransformCoord(focus, roZ);
	focus = XMVector3TransformCoord(focus, roX);
	focus = XMVector3TransformCoord(focus, roY);
	Vector3 focusnow = eye + focus;

	Vector3 focusnowUp = { XMVectorGetX(eye), XMVectorGetY(eye) + sin(XMVectorGetY(focus)), XMVectorGetZ(eye) };


	// �̵������� ���밪 ������ �ƴ� �����ǥ �������� �������� ������. focusnow�� ���� pos�� Nomalize���� �ʿ���.
	Vector3 front = XMVector3Normalize(focusnow - eye);	// ����
	// ������� ������� ��ǥ���� y�� 0���� �ʱ�ȭ
	front.y = 0.0f;
	// ����������� Y���� 90�� ȸ����Ų��. ���� �����ǥ�� ����
	Vector3 focusnowLeft = eye + XMVector3TransformCoord(front, XMMatrixRotationY(-XM_PIDIV2));

	if (KEY->Press('W')) eye += XMVector3Normalize(focusnow - eye) * CamSpeed * DELTA;
	if (KEY->Press('S')) eye -= XMVector3Normalize(focusnow - eye) * CamSpeed * DELTA;
	if (KEY->Press('A')) eye += XMVector3Normalize(focusnowLeft - eye) * CamSpeed * DELTA;
	if (KEY->Press('D')) eye -= XMVector3Normalize(focusnowLeft - eye) * CamSpeed * DELTA;
	if (KEY->Press('E')) eye += XMVector3Normalize(focusnowUp - eye) * CamSpeed * DELTA;
	if (KEY->Press('Q')) eye -= XMVector3Normalize(focusnowUp - eye) * CamSpeed * DELTA;

	Matrix view = XMMatrixLookAtLH(eye, focusnow, up);
	viewBuffer->Set(view);
	viewBuffer->SetVS(1);
}

// �������� �� �� z�� sin��ŭ ��������, x�� cos��ŭ �ö����.
// �������� �� �� z�� sin��ŭ ��������, x�� cos��ŭ ����������.