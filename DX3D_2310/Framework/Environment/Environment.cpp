#include "Framework.h"

Environment::Environment()
{
	CreateProjection();
	SetViewport();
}

Environment::~Environment()
{
	delete viewBuffer;
	delete projectionBuffer;
}

void Environment::Set()
{
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
	XMVECTOR eye = XMVectorSet(2, 8, -8, 0);//Cam Pos
	XMVECTOR focus = XMVectorSet(0, 0, 0, 0);//Cam Look at Pos
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
