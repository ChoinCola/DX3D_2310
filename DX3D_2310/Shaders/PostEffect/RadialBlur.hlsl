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
	float3 scale;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float2 radiusUV = input.uv - float2(0.5f, 0.5f);
	float r = length(radiusUV);
	radiusUV /= r;
	
	r = saturate(r / scale.y);
	
	float2 delta = -radiusUV * r * r * scale.z / scale.x;
	
	float4 result = 0;
	float2 uv = input.uv;
	
	for (int i = 0; i < scale.x; i++)
	{
		uv += delta;
		result += diffuseMap.Sample(samp, uv);
	}
	
	result /= scale.x;
	
	return result * mDiffuse;
}