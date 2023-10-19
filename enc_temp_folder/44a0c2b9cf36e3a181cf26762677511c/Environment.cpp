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
	// 마우스 회전함수. 마우스 value에 따라 카메라의 뷰포트 조정.

	// 현재 바라보는 방향은 Position기준 원형으로 둘러야한다.
	// Pitch (X 축 주위 회전)

	XMMATRIX roY = XMMatrixRotationX(XMConvertToRadians(-mousemovevalue.y));
	XMMATRIX roX = XMMatrixRotationY(XMConvertToRadians(+mousemovevalue.x));
	XMMATRIX roZ = XMMatrixRotationZ(XMConvertToRadians(+mousemovevalue.x));

	//focus = XMVector3TransformCoord(focus, roZ);
	focus = XMVector3TransformCoord(focus, roX);
	focus = XMVector3TransformCoord(focus, roY);
	Vector3 focusnow = eye + focus;

	Vector3 focusnowUp = { XMVectorGetX(eye), XMVectorGetY(eye) + sin(XMVectorGetY(focus)), XMVectorGetZ(eye) };


	// 이동방향은 절대값 기준이 아닌 상대좌표 기준으로 움직여야 함으로. focusnow와 현재 pos의 Nomalize값이 필요함.
	Vector3 front = XMVector3Normalize(focusnow - eye);	// 정면
	// 정면기준 쓸모없는 좌표값인 y를 0으로 초기화
	front.y = 0.0f;
	// 정면기준으로 Y축을 90도 회전시킨것. 그후 상대좌표로 갱신
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

// 우측으로 갈 때 z는 sin만큼 떨어지고, x는 cos만큼 올라야함.
// 좌측으로 갈 때 z는 sin만큼 떨어지고, x는 cos만큼 떨어져야함.