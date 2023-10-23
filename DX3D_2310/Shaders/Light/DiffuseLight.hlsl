//DiffuseLight.hlsl
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	// ������ ���� ��ǻ��� �Ѱ���.
	float3 normal : NORMAL;
};

PixelInput VS(VertexUVNormal input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	// ���� ����ȭ
	output.normal = normalize(mul(input.normal, (float3x3) world));

	return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	// �� ����ȭ
	float3 normal = normalize(input.normal);
	float3 light = normalize(lightDirection);
	
	// ���� �ݴ�� ������. dot���� �ٲ���.
	float diffuse = dot(normal, -light);
	return diffuseMap.Sample(samp, input.uv) * diffuse;
}