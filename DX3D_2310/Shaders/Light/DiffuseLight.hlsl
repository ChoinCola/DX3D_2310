//DiffuseLight.hlsl

cbuffer WordBuffer : register(b0)
{
	matrix world;
}

cbuffer ViewBuffer : register(b1)
{
	matrix view;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix projection;
}

cbuffer LightBuffer : register(b10)
{
	float3 lightDirection;
}

struct VertexInput
{
	float4 pos : POSITION;
	float2 uv : UV;
	// ����
	float3 normal : NORMAL;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	// ������ ���� ��ǻ��� �Ѱ���.
	float diffuse : DIFFUSE;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	// ���� ����ȭ
	float3 normal = normalize(input.normal);
	// �� ����ȭ
	float3 light = normalize(lightDirection);
	
	// ���� �ݴ�� ������. dot���� �ٲ���.
	output.diffuse = dot(normal, -light);
	
	return output;
}

Texture2D map : register(t0);
SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_TARGET
{
	return map.Sample(samp, input.uv) * input.diffuse;
}