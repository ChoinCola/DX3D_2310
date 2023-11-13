//PixelHeader.hlsli
#define MAX_LIGHT 100

// Map
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);
Texture2D depthBuffer : register(t3);

SamplerState samp : register(s0);

// Const Buffer
struct Light
{
	// ���� ����: RGB ���� �������� ���� ������ ��Ÿ���� 4���� �����Դϴ�.
	float4 color;
	
	// ���� ����: ���� ���ϴ� ������ ��Ÿ���� 3���� �����Դϴ�. �ַ� Directional Light�� Spot Light���� ���˴ϴ�.
	float3 direction;
	
	// ���� Ÿ���� ���
	int type;
	
	// ���� ��ġ��: 3D �������� ���� ��ġ�� ��Ÿ���� 3���� �����Դϴ�. �ַ� Point Light�� Spot Light���� ���˴ϴ�.
	float3 position;
	
	// ���� �Ÿ�: ���� ������ �� �ִ� �ִ� �Ÿ��� ��Ÿ���ϴ�. �� ���� �ַ� Spot Light�� Point Light���� ���˴ϴ�.
	float range;
	
	// ���� ���ΰ���: Spot Light���� ���� ���� ���η� ���� ������ ��Ÿ���ϴ�. �� ���� Spot Light������ ���˴ϴ�.
	float inner;
	
	// ���� �ܺΰ���: Spot Light���� ���� ���� �ܺη� ������ ������ ��Ÿ���ϴ�. �� ���� Spot Light������ ���˴ϴ�.
	float outer;
	
	// ���̴��� �������� ���������� Ȯ��
	int isActive;
	
	// ����������
	float attentionIntensity;
};
cbuffer LightBuffer : register(b0)
{
	Light lights[MAX_LIGHT];
	// ������ ����
	int lightCount;
	// ������
	float3 ambientLight;
    // õ�尣����. õ��κ��� �ݻ������� �����ش�.
	float3 ambientCeil;
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
struct Material
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

Material GetMaterial(LightPixelInput input)
{
	Material material;
	material.normal = GetNormal(input.tangent,
		input.binormal, input.normal, input.uv);
	// �⺻����
	material.baseColor = diffuseMap.Sample(samp, input.uv);
	// ���ݻ� ����
	material.specularIntensity = specularMap.Sample(samp, input.uv);
	// ������ ����� ��ġ
	material.worldPos = input.worldPos;
	// ���ݻ� ������ ī�޶� ����
	material.viewDir = normalize(input.worldPos - input.viewPos);
	
	return material;
}

// ������ ����
float4 CalcAmbient(Material material)
{
	float up = material.normal.y * -0.5f;
	
	float4 ambient = float4(ambientLight + up * ambientCeil, 1);
	
	return material.baseColor * ambient * mAmbient;
}

// �ڿ���, �¾籤�� �ǹ��Ѵ�.
float4 CalcDirectional(Material material, Light light)
{
	float3 toLight = normalize(light.direction);
	
	float diffuseInstensity = saturate(dot(material.normal, -toLight));
	float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
	if (diffuseInstensity > 0)
	{
		float3 halfWay = normalize(material.viewDir + toLight);
		float specular = saturate(dot(material.normal, -halfWay));

		finalColor += light.color * pow(specular, shininess) *
		material.specularIntensity * mSpecular;
	}

	return finalColor * material.baseColor;
}

// ����Ʈ ����Ʈ�� ����ϴ� �Լ��Դϴ�.
float4 CalcPoint(Material material, Light light)
{
	// �������� ������ ����մϴ�.
	float3 toLight = material.worldPos - light.position;
	// �������� �Ÿ��� ����մϴ�.
	float distanceToLight = length(toLight);
	// �������� ������ ����ȭ�մϴ�.
	toLight /= distanceToLight;
	
	// Ȯ�� ������ ����մϴ�.
	float diffuseInstensity = saturate(dot(material.normal, -toLight));
	// ���� ������ ���� ����� Ȯ�� ������ ���Դϴ�.
	float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
	// Ȯ�� ������ 0���� ū ��쿡�� �ݻ縦 ����մϴ�.
	if (diffuseInstensity > 0)
	{
		// �ݻ縦 ����ϱ� ���� �߰� ������ ����մϴ�.
		float3 halfWay = normalize(material.viewDir + toLight);
		float specular = saturate(dot(material.normal, -halfWay));

		finalColor += light.color * pow(specular, shininess) *
		material.specularIntensity * mSpecular;
	}
	
	// �������� �Ÿ��� ����ȭ�մϴ�.
	float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
	// ���踦 ����մϴ�.
	float attention = pow(distanceToLightNormal, light.attentionIntensity);

	// ���� ������ �⺻ ����� ������ ���Դϴ�.
	return finalColor * material.baseColor * attention;
}

// ����Ʈ����Ʈ�� ����ϴ� �Լ��Դϴ�.
float4 CalcSpot(Material material, Light light)
{
	// �������� ������ ����մϴ�.
	float3 toLight = material.worldPos - light.position;
	// �������� �Ÿ��� ����մϴ�.
	float distanceToLight = length(toLight);
	// �������� ������ ����ȭ�մϴ�. normalize���� �ʰ� ������ �Ÿ����� ����������� ��.
	toLight /= distanceToLight;
	
	// Ȯ�� ������ ����մϴ�. Dot�� ��ֺ��� ���� ��������� ������ ���. �Ѵ� normalize�Ǿ������� cos�� ���´�.
	// saturate�Լ��� ��������͸� 0 ���� ������ 0 1���� ũ�� 1�� �����Ѵ�.
	float diffuseInstensity = saturate(dot(material.normal, -toLight));
	
	// ���� ������ ���� ����� Ȯ�� ������ ���Դϴ�.
	float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
	// Ȯ�� ������ 0���� ū ��쿡�� �ݻ縦 ����մϴ�. ���� ǥ�鿡 �����ϴ��� ���� Ȯ��
	if (diffuseInstensity > 0)
	{
		// �ݻ縦 ����ϱ� ���� �߰� ������ ����մϴ�.
		float3 halfWay = normalize(material.viewDir + toLight);
		float specular = saturate(dot(material.normal, -halfWay));

		finalColor += light.color * pow(specular, shininess) *
		material.specularIntensity * mSpecular;
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
	conAttention = pow(conAttention, light.attentionIntensity);
	
	// �������� �Ÿ��� ����ȭ�մϴ�.
	float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
	// ���踦 ����մϴ�.
	float attention = pow(distanceToLightNormal, light.attentionIntensity);

	// ���� ������ �⺻ ����� ���� �� ������ ������ ���Դϴ�.
	// �������� * ���� ���� * ���� ��������(0 ~ 1 ������ flaot��)(���� ��ü�� �Ÿ��� �ش�) * ���� �ܰ����� ���� ���� ��������
	return finalColor * material.baseColor * attention * conAttention;
}