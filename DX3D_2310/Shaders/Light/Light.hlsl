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

// �ȼ� ���̴� �Լ��Դϴ�.
float4 PS(LightPixelInput input) : SV_TARGET
{
	// ���� �����͸� �����ɴϴ�.
	LightData lightData = GetLightData(input);
	
	// �ֺ����� ����մϴ�.
	float4 ambient = CalcAmbient(lightData);
	//float4 result = CalcDirectional(lightData, light);
	// ������ ���� ���� ����� ����մϴ�. 
	// ���⼭�� ����Ʈ ����Ʈ�� ����Ͽ����ϴ�.
	float4 result = CalcPoint(lightData, light);
	//float4 result = CalcSpot(lightData, light);
	// �ֺ����� ���� ����� ���Ͽ� ��ȯ�մϴ�.
	return ambient + result;
}