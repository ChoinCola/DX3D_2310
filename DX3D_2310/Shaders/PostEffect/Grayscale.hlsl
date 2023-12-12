//Tutorial.hlsl
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
	float4 baseColore = diffuseMap.Sample(samp, input.uv);
	
	//float scale = (baseColore.r + baseColore.g + baseColore.b) / 3;
	//float3 grayColor;
	//grayColor.r = baseColore.r * 0.3f;
	//grayColor.g = baseColore.g * 0.59f;
	//grayColor.b = baseColore.b * 0.11f;
	
	float scale = dot(baseColore.rgb, float3(0.3f, 0.59f, 0.11f));
	
	return float4(scale.xxx, 1.0f) * mDiffuse;
}