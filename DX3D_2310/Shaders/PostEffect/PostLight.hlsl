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
cbuffer ValueBuffer : register(b10)
{
	float range;
	float2 imageSize;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 baseColor = diffuseMap.Sample(samp, input.uv);
	
	float2 pixelCoord = input.uv * imageSize;
	
	float dist = distance(pixelCoord, imageSize * 0.5f);
	
	float3 color = baseColor.rgb - dist / range;
	
	return float4(color, baseColor.a) * mDiffuse;
}