//DiffuseLight.hlsl
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	//�浹�� ������ ��ġ�� �Ѱ��ش�.
	float4 pos : SV_POSITION;
};

PixelInput VS(Vertex input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	// vertices���� ��ġ�� ������.
	return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
	// �ܼ� �÷��� ����Ѵ�.
	// ������ collision�� �Ұ� �̱� ����.
	return mDiffuse;
}