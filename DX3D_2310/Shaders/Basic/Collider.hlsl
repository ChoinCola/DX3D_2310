//DiffuseLight.hlsl
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	//충돌용 임으로 위치만 넘겨준다.
	float4 pos : SV_POSITION;
};

PixelInput VS(Vertex input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	// vertices에서 위치만 가져옴.
	return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
	// 단순 컬러만 출력한다.
	// 어차피 collision만 할것 이기 때문.
	return mDiffuse;
}