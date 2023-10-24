//DiffuseLight.hlsl
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	// 연산이 끝난 디퓨즈값을 넘겨줌.
	float3 normal : NORMAL;
	float3 worldPos : POSITION;
};

PixelInput VS(VertexUVNormal input)
{
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.worldPos = output.pos;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = normalize(mul(input.normal, (float3x3) world));
	
	return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	// 빛 정규화
	float3 normal = normalize(input.normal);
	float3 light = normalize(lightDirection);
	float3 viewDir = normalize(input.worldPos - invView._41_42_43);
	// 빛을 반대로 뒤집고. dot으로 바꿔줌.
	// saturate 는 데이터의 제한범위 설정하는것.
	float diffuse = saturate(dot(normal, -light));
	float4 specular = 0;
	
	if(diffuse > 0)
	{
		// Phong Shading, 이걸 바탕으로 만든게
		//float3 reflection = normalize(reflect(light, normal));
		//specular = saturate(dot(-reflection, viewDir));
		
		// 응용.
		// Bllinn Phong Shading		
		float3 reflection = normalize(reflect(light, normal));
		specular = saturate(dot(-reflection, viewDir));
		
		float4 specularIntensity = specularMap.Sample(samp, input.uv);
		
		specular = pow(specular, shininess) * specularIntensity;
	}
	
	return diffuseMap.Sample(samp, input.uv) * diffuse + specular.xxxx;
}