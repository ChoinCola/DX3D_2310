//DiffuseLight.hlsl
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	// ������ ���� ��ǻ��� �Ѱ���.
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
	// �� ����ȭ
	float3 normal = normalize(input.normal);
	float3 light = normalize(lightDirection);
	float3 viewDir = normalize(input.worldPos - invView._41_42_43);
	// ���� �ݴ�� ������. dot���� �ٲ���.
	// saturate �� �������� ���ѹ��� �����ϴ°�.
	float diffuse = saturate(dot(normal, -light));
	float specular = 0;
	
	if(diffuse > 0)
	{
		//Phong Shading
		float3 reflection = normalize(reflect(light, normal));
		specular = saturate(dot(-reflection, viewDir));
		
		specular = pow(specular, shininess);
	}
	return diffuseMap.Sample(samp, input.uv) * diffuse + specular.xxxx;
}