//PixelHeader.hlsli
#define MAX_LIGHT 100

// Map
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

// Const Buffer
struct Light
{
	// 빛의 색상: RGB 색상 공간에서 빛의 색상을 나타내는 4차원 벡터입니다.
	float4 color;
	
	// 빛의 방향: 빛이 향하는 방향을 나타내는 3차원 벡터입니다. 주로 Directional Light나 Spot Light에서 사용됩니다.
	float3 direction;
	
	// 빛의 타입이 어떤지
	int type;
	
	// 빛의 위치값: 3D 공간에서 빛의 위치를 나타내는 3차원 벡터입니다. 주로 Point Light나 Spot Light에서 사용됩니다.
	float3 position;
	
	// 빛의 거리: 빛이 도달할 수 있는 최대 거리를 나타냅니다. 이 값은 주로 Spot Light나 Point Light에서 사용됩니다.
	float range;
	
	// 원뿔 내부각도: Spot Light에서 빛이 원뿔 내부로 들어가는 각도를 나타냅니다. 이 값은 Spot Light에서만 사용됩니다.
	float inner;
	
	// 원뿔 외부각도: Spot Light에서 빛이 원뿔 외부로 나가는 각도를 나타냅니다. 이 값은 Spot Light에서만 사용됩니다.
	float outer;
	
	// 쉐이더가 켜지는지 안켜지는지 확인
	int isActive;
	
	// 감쇠조절값
	float attentionIntensity;
};
cbuffer LightBuffer : register(b0)
{
	Light lights[MAX_LIGHT];
	// 빛연산 계산수
	int lightCount;
	// 간접광
	float3 ambientLight;
    // 천장간접광. 천장부분의 반사정도를 따로준다.
	float3 ambientCeil;
}

cbuffer MaterialBuffer : register(b2)
{
	// 재질
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
	// 연산이 끝난 디퓨즈값을 넘겨줌.
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
	// 기본색상
	material.baseColor = diffuseMap.Sample(samp, input.uv);
	// 정반사 강도
	material.specularIntensity = specularMap.Sample(samp, input.uv);
	// 정점의 월드상 위치
	material.worldPos = input.worldPos;
	// 정반사 구현시 카메라 방향
	material.viewDir = normalize(input.worldPos - input.viewPos);
	
	return material;
}

// 간접광 제작
float4 CalcAmbient(Material material)
{
	float up = material.normal.y * -0.5f;
	
	float4 ambient = float4(ambientLight + up * ambientCeil, 1);
	
	return material.baseColor * ambient * mAmbient;
}

// 자연광, 태양광을 의미한다.
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

// 포인트 라이트를 계산하는 함수입니다.
float4 CalcPoint(Material material, Light light)
{
	// 빛으로의 방향을 계산합니다.
	float3 toLight = material.worldPos - light.position;
	// 빛까지의 거리를 계산합니다.
	float distanceToLight = length(toLight);
	// 빛으로의 방향을 정규화합니다.
	toLight /= distanceToLight;
	
	// 확산 강도를 계산합니다.
	float diffuseInstensity = saturate(dot(material.normal, -toLight));
	// 최종 색상은 빛의 색상과 확산 강도의 곱입니다.
	float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
	// 확산 강도가 0보다 큰 경우에만 반사를 계산합니다.
	if (diffuseInstensity > 0)
	{
		// 반사를 계산하기 위해 중간 방향을 계산합니다.
		float3 halfWay = normalize(material.viewDir + toLight);
		float specular = saturate(dot(material.normal, -halfWay));

		finalColor += light.color * pow(specular, shininess) *
		material.specularIntensity * mSpecular;
	}
	
	// 빛까지의 거리를 정규화합니다.
	float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
	// 감쇠를 계산합니다.
	float attention = pow(distanceToLightNormal, light.attentionIntensity);

	// 최종 색상은 기본 색상과 감쇠의 곱입니다.
	return finalColor * material.baseColor * attention;
}

// 스포트라이트를 계산하는 함수입니다.
float4 CalcSpot(Material material, Light light)
{
	// 빛으로의 방향을 계산합니다.
	float3 toLight = material.worldPos - light.position;
	// 빛까지의 거리를 계산합니다.
	float distanceToLight = length(toLight);
	// 빛으로의 방향을 정규화합니다. normalize하지 않고도 어차피 거리값을 나누어버리면 됨.
	toLight /= distanceToLight;
	
	// 확산 강도를 계산합니다. Dot은 노멀벡터 빛의 방향사이의 점곱을 계산. 둘다 normalize되었음으로 cos만 남는다.
	// saturate함수는 결과데이터를 0 보다 작으면 0 1보다 크면 1로 고정한다.
	float diffuseInstensity = saturate(dot(material.normal, -toLight));
	
	// 최종 색상은 빛의 색상과 확산 강도의 곱입니다.
	float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
	// 확산 강도가 0보다 큰 경우에만 반사를 계산합니다. 빛이 표면에 도달하는지 정도 확인
	if (diffuseInstensity > 0)
	{
		// 반사를 계산하기 위해 중간 방향을 계산합니다.
		float3 halfWay = normalize(material.viewDir + toLight);
		float specular = saturate(dot(material.normal, -halfWay));

		finalColor += light.color * pow(specular, shininess) *
		material.specularIntensity * mSpecular;
	}
	
	// 빛의 방향을 정규화합니다.
	float3 dir = normalize(light.direction);
	// 빛의 방향과 표면으로의 빛방향에 대한 cos값을 저장한다.
	float cosAngle = dot(dir, toLight);
	
	// 외부 및 내부 각도를 계산합니다. 내부원뿔과 외부원뿔을 의미한다.
	float outer = cos(radians(light.outer));
	float inner = 1.0f / cos(radians(light.inner));
	
	// 스포트라이트의 원뿔형 감쇠를 계산합니다. 
	// 물체표면과 표면에 대한 빛의 Cos값에 외곽원뿔 각도를 빼주는 이유는, 각도 외부의 vertice에 대해서는 빛연산을 하지 않기 위해서 이다.
	
	// inner를 곱하는 이유는. 해당값에 내부원뿔의 각도를 보정해주어 내부원뿔 일 경우, 1을 출력하게 만들기 위해서 이다.
	float conAttention = saturate((cosAngle - outer) * inner);
	// 빛의 감쇠작용시 데이터의 곡선파형을 위해 제곱승을 사용한다.
	conAttention = pow(conAttention, light.attentionIntensity);
	
	// 빛까지의 거리를 정규화합니다.
	float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
	// 감쇠를 계산합니다.
	float attention = pow(distanceToLightNormal, light.attentionIntensity);

	// 최종 색상은 기본 색상과 감쇠 및 원뿔형 감쇠의 곱입니다.
	// 최종색상 * 빛의 색상 * 빛의 감쇠정도(0 ~ 1 사이의 flaot값)(빛과 물체의 거리에 해당) * 원의 외곽선에 대한 빛의 감쇠정도
	return finalColor * material.baseColor * attention * conAttention;
}