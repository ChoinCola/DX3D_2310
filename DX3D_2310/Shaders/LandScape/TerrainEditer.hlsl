#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexInput
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 alpha : ALPHA;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPos : POSITION0;
	float3 viewPos : POSITION1;
	float4 alpha : ALPHA;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.worldPos = output.pos;
	output.viewPos = invView._41_42_43;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	output.alpha = input.alpha;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = float3(1, 0, 0);
	output.binormal = float3(0, 1, 0);
	
	return output;
}

cbuffer BrushBuffer : register(b10)
{
	int type;
	float3 pickingPos;
	
	float range;
	float3 color;
}

float4 BrushColor(float3 pos)
{
	float2 direction = pos.xz - pickingPos.xz;
	
	float dist = length(direction);
	
	if (dist < range)
		return float4(color, 0);

	return float4(0, 0, 0, 0);
}

Texture2D secondDiffuseMap : register(t11);
Texture2D thirdDiffuseMap : register(t12);

float4 PS(PixelInput input) : SV_TARGET
{
	Material material;
	material.normal = GetNormal(input.tangent,
		input.binormal, input.normal, input.uv);
	material.baseColor = diffuseMap.Sample(samp, input.uv);
	material.specularIntensity = specularMap.Sample(samp, input.uv);
	material.worldPos = input.worldPos;
	material.viewDir = normalize(input.worldPos - input.viewPos);
	
	float4 second = secondDiffuseMap.Sample(samp, input.uv);
	float4 third = thirdDiffuseMap.Sample(samp, input.uv);
	
	material.baseColor = lerp(material.baseColor, second, input.alpha.r);
	material.baseColor = lerp(material.baseColor, third, input.alpha.g);
	
	float4 ambient = CalcAmbient(material);
	float4 result = 0;
	
	for (int i = 0; i < lightCount; i++)
	{
		if (!lights[i].isActive)
			continue;
	
		if (lights[i].type == 0)
			result += CalcDirectional(material, lights[i]);
		else if (lights[i].type == 1)
			result += CalcPoint(material, lights[i]);
		else if (lights[i].type == 2)
			result += CalcSpot(material, lights[i]);
	}
	
	float4 brush = BrushColor(input.worldPos);
	
	return ambient + result + mEmissive + brush;
}