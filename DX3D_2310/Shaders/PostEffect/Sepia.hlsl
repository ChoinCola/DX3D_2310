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
	
	float4 sepia = 1;
	
	sepia.r = dot(baseColore.rgb, float3(0.393f, 0.769f, 0.189f));
	sepia.g = dot(baseColore.rgb, float3(0.349f, 0.646f, 0.168f));
	sepia.b = dot(baseColore.rgb, float3(0.277f, 0.534f, 0.131f));
	
	return sepia * mDiffuse;
}