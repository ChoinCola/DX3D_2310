//SpecularLight
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

// 꼭짓점 셰이더 함수입니다.
LightPixelInput VS(VertexUVNormalTangent input)
{
	LightPixelInput output;
	// 입력 꼭짓점을 월드 공간으로 변환합니다.
	output.pos = mul(input.pos, world);
	// 월드 공간에서의 위치를 저장합니다.
	output.worldPos = output.pos;
	// 뷰 공간에서의 위치를 저장합니다.
	output.viewPos = invView._41_42_43;
	
	// 위치를 뷰 공간으로 변환합니다.
	output.pos = mul(output.pos, view);
	// 위치를 투영 공간으로 변환합니다.
	output.pos = mul(output.pos, projection);
	
	// UV 좌표를 저장합니다.
	output.uv = input.uv;
	
	// 노멀과 탄젠트를 월드 공간으로 변환하고, 
	// 빈노멀(노멀과 탄젠트에 수직인 벡터)을 계산합니다.
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}
// 레이드 할 때 스킬쓰기전 범위 표시. 할때 많이 사용하는 방법이다.
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

// 픽셀 셰이더 함수입니다.
float4 PS(LightPixelInput input) : SV_TARGET
{
	// 광원 데이터를 가져옵니다.
	Material material = GetMaterial(input);
	
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
