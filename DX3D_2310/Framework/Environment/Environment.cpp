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
	XMVECTOR eye = XMVectorSet(3, 50, -50, 0);//Cam Pos
	XMVECTOR focus = XMVectorSet(125, 0, 125, 0);//Cam Look at Pos
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);//Cam Up Vector

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

}
