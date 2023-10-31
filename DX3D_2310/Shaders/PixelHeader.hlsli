//PixelHeader.hlsli

// Map
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

// Const Buffer
struct Light
{
	// ���� ����: RGB ���� �������� ���� ������ ��Ÿ���� 4���� �����Դϴ�.
	float4 color;
	
	// ���� ����: ���� ���ϴ� ������ ��Ÿ���� 3���� �����Դϴ�. �ַ� Directional Light�� Spot Light���� ���˴ϴ�.
	float3 direction;
	
	// �����Ⱚ
	float padding;
	
	// ���� ��ġ��: 3D �������� ���� ��ġ�� ��Ÿ���� 3���� �����Դϴ�. �ַ� Point Light�� Spot Light���� ���˴ϴ�.
	float3 position;
	
	// ���� �Ÿ�: ���� ������ �� �ִ� �ִ� �Ÿ��� ��Ÿ���ϴ�. �� ���� �ַ� Spot Light�� Point Light���� ���˴ϴ�.
	float range;
	
	// ���� ���ΰ���: Spot Light���� ���� ���� ���η� ���� ������ ��Ÿ���ϴ�. �� ���� Spot Light������ ���˴ϴ�.
	float inner;
	
	// ���� �ܺΰ���: Spot Light���� ���� ���� �ܺη� ������ ������ ��Ÿ���ϴ�. �� ���� Spot Light������ ���˴ϴ�.
	float outer;
};
cbuffer LightBuffer : register(b0)
{
	Light light;
	// ������
	float4 ambientLight;
    // õ�尣����. õ��κ��� �ݻ������� �����ش�.
	float4 ambientCeil;
}

cbuffer MaterialBuffer : register(b2)
{
	// ����
	float4 mDiffuse;
	float4 mSpecular;
	float4 mAmbient;
	float4 mEmissive;
	
	float shininess;
	int hasNormalMap;
}

// Struct
struct LightData
{
	float3 normal;
	float4 baseColor;
	float4 specularIntensity;
	float3 worldPos;
	float3 viewDir;
};

struct LightPixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	// ������ ���� ��ǻ��� �Ѱ���.
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPos : POSITION0;
	float3 viewPos : POSITION1;
};

// Function
float3 GetNormal(float3 T, float3 B, float3 N, float2 uv)
{
	T = normalize(T);
	B = normalize(B);
	N = normalize(N);

	if (hasNormalMap)
	{
		float3 normal = normalMap.Sample(samp, uv).rgb;
		normal = normal * 2.0f - 1.0f; //0~1 -> -1~1
		float3x3 TBN = float3x3(T, B, N);
		N = normalize(mul(normal, TBN));
	}
	
	
	return N;
};

LightData GetLightData(LightPixelInput input)
{
	LightData lightData;
	lightData.normal = GetNormal(input.tangent,
		input.binormal, input.normal, input.uv);
	lightData.baseColor = diffuseMap.Sample(samp, input.uv);
	lightData.specularIntensity = specularMap.Sample(samp, input.uv);
	lightData.worldPos = input.worldPos;
	lightData.viewDir = normalize(input.worldPos - input.viewPos);
	
	return lightData;
}

// ������ ����
float4 CalcAmbient(LightData lightData)
{
	float up = lightData.normal.y * 0.5f + 0.5f;
	
	float4 ambient = ambientLight + up * ambientCeil;
	
	return lightData.baseColor * ambient * mAmbient;
}

float4 CalcDirectional(LightData lightData, Light light)
{
	float3 toLight = normalize(light.direction);
	
	float diffuseInstensity = saturate(dot(lightData.normal, -toLight));
	float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
	if (diffuseInstensity > 0)
	{
		float3 halfWay = normalize(lightData.viewDir + toLight);
		float specular = saturate(dot(lightData.normal, -halfWay));

		finalColor += light.color * pow(specular, shininess) *
		lightData.specularIntensity * mSpecular;
	}

	return finalColor * lightData.baseColor;
}

// ����Ʈ ����Ʈ�� ����ϴ� �Լ��Դϴ�.
float4 CalcPoint(LightData lightData, Light light)
{
	// �������� ������ ����մϴ�.
	float3 toLight = lightData.worldPos - light.position;
	// �������� �Ÿ��� ����մϴ�.
	float distanceToLight = length(toLight);
	// �������� ������ ����ȭ�մϴ�.
	toLight /= distanceToLight;
	
	// Ȯ�� ������ ����մϴ�.
	float diffuseInstensity = saturate(dot(lightData.normal, -toLight));
	// ���� ������ ���� ����� Ȯ�� ������ ���Դϴ�.
	float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
	// Ȯ�� ������ 0���� ū ��쿡�� �ݻ縦 ����մϴ�.
	if (diffuseInstensity > 0)
	{
		// �ݻ縦 ����ϱ� ���� �߰� ������ ����մϴ�.
		float3 halfWay = normalize(lightData.viewDir + toLight);
		float specular = saturate(dot(lightData.normal, -halfWay));

		finalColor += light.color * pow(specular, shininess) *
		lightData.specularIntensity * mSpecular;
	}
	
	// �������� �Ÿ��� ����ȭ�մϴ�.
	float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
	// ���踦 ����մϴ�.
	float attention = pow(distanceToLightNormal, 2);

	// ���� ������ �⺻ ����� ������ ���Դϴ�.
	return finalColor * lightData.baseColor * attention;
}

// ����Ʈ����Ʈ�� ����ϴ� �Լ��Դϴ�.
float4 CalcSpot(LightData lightData, Light light)
{
	// �������� ������ ����մϴ�.
	float3 toLight = lightData.worldPos - light.position;
	// �������� �Ÿ��� ����մϴ�.
	float distanceToLight = length(toLight);
	// �������� ������ ����ȭ�մϴ�. normalize���� �ʰ� ������ �Ÿ����� ����������� ��.
	toLight /= distanceToLight;
	
	// Ȯ�� ������ ����մϴ�. Dot�� ��ֺ��� ���� ��������� ������ ���. �Ѵ� normalize�Ǿ������� cos�� ���´�.
	// saturate�Լ��� ��������͸� 0 ���� ������ 0 1���� ũ�� 1�� �����Ѵ�.
	float diffuseInstensity = saturate(dot(lightData.normal, -toLight));
	
	// ���� ������ ���� ����� Ȯ�� ������ ���Դϴ�.
	float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
	// Ȯ�� ������ 0���� ū ��쿡�� �ݻ縦 ����մϴ�. ���� ǥ�鿡 �����ϴ��� ���� Ȯ��
	if (diffuseInstensity > 0)
	{
		// �ݻ縦 ����ϱ� ���� �߰� ������ ����մϴ�.
		float3 halfWay = normalize(lightData.viewDir + toLight);
		float specular = saturate(dot(lightData.normal, -halfWay));

		finalColor += light.color * pow(specular, shininess) *
		lightData.specularIntensity * mSpecular;
	}
	
	// ���� ������ ����ȭ�մϴ�.
	float3 dir = normalize(light.direction);
	// ���� ����� ǥ�������� �����⿡ ���� cos���� �����Ѵ�.
	float cosAngle = dot(dir, toLight);
	
	// �ܺ� �� ���� ������ ����մϴ�. ���ο��԰� �ܺο����� �ǹ��Ѵ�.
	float outer = cos(radians(light.outer));
	float inner = 1.0f / cos(radians(light.inner));
	
	// ����Ʈ����Ʈ�� ������ ���踦 ����մϴ�. 
	// ��üǥ��� ǥ�鿡 ���� ���� Cos���� �ܰ����� ������ ���ִ� ������, ���� �ܺ��� vertice�� ���ؼ��� �������� ���� �ʱ� ���ؼ� �̴�.
	// inner�� ���ϴ� ������. �ش簪�� ���ο����� ������ �������־� ���ο��� �� ���, 1�� ����ϰ� ����� ���ؼ� �̴�.
	float conAttention = saturate((cosAngle - outer) * inner);
	// ���� �����ۿ�� �������� ������� ���� �������� ����Ѵ�.
	conAttention = pow(conAttention, 2);
	
	// �������� �Ÿ��� ����ȭ�մϴ�.
	float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
	// ���踦 ����մϴ�.
	float attention = pow(distanceToLightNormal, 2);

	// ���� ������ �⺻ ����� ���� �� ������ ������ ���Դϴ�.
	// �������� * ���� ���� * ���� ��������(0 ~ 1 ������ flaot��)(���� ��ü�� �Ÿ��� �ش�) * ���� �ܰ����� ���� ���� ��������
	return finalColor * lightData.baseColor * attention * conAttention;
}