//DiffuseLight.hlsl
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	// 연산이 끝난 디퓨즈값을 넘겨줌.
	float3 normal : NORMAL;
};

PixelInput VS(VertexUVNormal input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	// 법선 정규화
	output.normal = normalize(mul(input.normal, (float3x3) world));

	return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	// 빛 정규화
	float3 normal = normalize(input.normal);
	float3 light = normalize(lightDirection);
	
	// 빛을 반대로 뒤집고. dot으로 바꿔줌.
	float diffuse = dot(normal, -light);
	return diffuseMap.Sample(samp, input.uv) * diffuse;
}