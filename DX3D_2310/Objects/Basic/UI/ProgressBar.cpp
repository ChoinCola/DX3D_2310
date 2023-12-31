#include "Framework.h"

ProgressBar::ProgressBar(wstring frontImageFile, wstring backImageFile)
	: Quad(frontImageFile)
{
	material->SetShader(L"UI/ProgressBar.hlsl");

	backImage = Texture::Add(backImageFile);

	valueBuffer = new FloatValueBuffer();
	valueBuffer->GetData()[0] = 1.0f;
}

ProgressBar::~ProgressBar()
{
	delete valueBuffer;
}

void ProgressBar::Render()
{
	valueBuffer->SetPS(10);
	backImage->PSSet(10);

	Quad::Render();
}

void ProgressBar::SetAmount(float value)
{
	valueBuffer->GetData()[0] = value;
}
