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

// 픽셀 셰이더 함수입니다.
float4 PS(LightPixelInput input) : SV_TARGET
{
	// 깊이값 참조
	//float depthValue = depthBuffer.Sample(samp, input.pos.xy).r;
	//float nowdepth = input.pos.z / input.pos.w;
	//// 깊이값 확인
	//if (nowdepth > depthValue)
	//	discard;
		// 광원 데이터를 가져옵니다.
		Material material = GetMaterial(input);
	
	// 주변광을 계산합니다.
	//float4 ambient = CalcAmbient(material);
		float4 ambient = 0;
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
