//SpecularLight
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

// ������ ���̴� �Լ��Դϴ�.
LightPixelInput VS(VertexUVNormalTangent input)
{
	LightPixelInput output;
	// �Է� �������� ���� �������� ��ȯ�մϴ�.
	output.pos = mul(input.pos, world);
	// ���� ���������� ��ġ�� �����մϴ�.
	output.worldPos = output.pos;
	// �� ���������� ��ġ�� �����մϴ�.
	output.viewPos = invView._41_42_43;
	
	// ��ġ�� �� �������� ��ȯ�մϴ�.
	output.pos = mul(output.pos, view);
	// ��ġ�� ���� �������� ��ȯ�մϴ�.
	output.pos = mul(output.pos, projection);
	
	// UV ��ǥ�� �����մϴ�.
	output.uv = input.uv;
	
	// ��ְ� ź��Ʈ�� ���� �������� ��ȯ�ϰ�, 
	// ����(��ְ� ź��Ʈ�� ������ ����)�� ����մϴ�.
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

Texture2D alphaMap : register(t10);
Texture2D secondMap : register(t11);

// �ȼ� ���̴� �Լ��Դϴ�.
float4 PS(LightPixelInput input) : SV_TARGET
{
	// ���� �����͸� �����ɴϴ�.
	Material material = GetMaterial(input);
	float4 alpha = alphaMap.Sample(samp, input.uv);
	float4 second = secondMap.Sample(samp, input.uv);
	
	material.baseColor = lerp(material.baseColor, second, alpha.r);
	
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
	
	return ambient + result + mEmissive;
}
