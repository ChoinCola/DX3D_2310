//DiffuseLight.hlsl
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	// 연산이 끝난 디퓨즈값을 넘겨줌.
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPos : POSITION;
};

PixelInput VS(VertexUVNormalTangent input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.worldPos = output.pos;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	
	float3 T = normalize(input.tangent);
	float3 B = normalize(input.binormal);
	float3 N = normalize(input.normal);
	
	// 빛 정규화 작업.
	float3 normal = N;
	float3 light = normalize(lightDirection);
	float3 viewDir = normalize(input.worldPos - invView._41_42_43);
	
	float3 normalMapColor = normalMap.Sample(samp, input.uv).rgb;
	normal = normalMapColor * 2.0f - 1.0f; // 0~1 -> -1~1
	float3x3 TBN = float3x3(T, B, N);
	normal = normalize(mul(normal, TBN));
	
	// 빛을 반대로 뒤집고. dot으로 바꿔줌.
	// saturate 는 데이터의 제한범위 설정하는것.
	
	float diffsue = saturate(dot(normal, -light));
	float4 specular = 0;
	
	if (diffsue > 0)
	{
		//Blinn Phong Shading
		float3 halfWay = normalize(viewDir + light);
		specular = saturate(dot(normal, -halfWay));
		
		float4 specularIntensity = specularMap.Sample(samp, input.uv);
		
		specular = pow(specular, shininess) * specularIntensity;
	}
	
	float4 ambient = albedo * 0.1f;
	
	return diffuseMap.Sample(samp, input.uv) * diffsue + specular + ambient;
}