// VertexHeader.hlsli

//VertexLayouts
struct Vertex
{
	float4 pos : POSITION;
};

struct VertexUV
{
	float4 pos : POSITION;
	float2 uv : UV;
};

struct VertexColor
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VertexUVNormal
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	
};

struct VertexUVNormalTangentBlend
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 indices : BLENDINDICES;
	// 특정 본들, 본에 꼽혀있을 때, 어떤 본에 가중치를 줄 것인지 웨이트가 들어감.
	float4 weights : BLENDWEIGHTS;
};

////////////////////////////////////////////////////

cbuffer WordBuffer : register(b0)
{
	matrix world;
}

cbuffer ViewBuffer : register(b1)
{
	matrix view;
	matrix invView;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix projection;
}

struct Frame
{
	int clip;
	int curFrame;
	float time;
	float scale; // 애니메이션 재생 속도 조절용 스케일
};

struct Motion
{
	float takeTime; // 보간시간
	float tweenTime; // 계산 시간
	float running_time;
	float padding;
	
	Frame cur, next;
};

cbuffer FrameBuffer : register(b3)
{
	Motion motion;
}

Texture2DArray transformMap : register(t0);

// 회전 값을 이용하여 회전 행렬을 생성하는 함수
float4x4 CreateRotationMatrix(float3 rotation)
{
	float4x4 rotationMatrix = float4x4(
        cos(rotation.y) * cos(rotation.z), -cos(rotation.y) * sin(rotation.z), sin(rotation.y), 0.0f,
        sin(rotation.x) * sin(rotation.y) * cos(rotation.z) + cos(rotation.x) * sin(rotation.z), cos(rotation.x) * cos(rotation.z) - sin(rotation.x) * sin(rotation.y) * sin(rotation.z), -sin(rotation.x) * cos(rotation.y), 0.0f,
        -cos(rotation.x) * sin(rotation.y) * cos(rotation.z) + sin(rotation.x) * sin(rotation.z), cos(rotation.x) * sin(rotation.z) + sin(rotation.x) * sin(rotation.y) * cos(rotation.z), cos(rotation.x) * cos(rotation.y), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

	return rotationMatrix;
}

// 스케일 값을 이용하여 스케일 행렬을 생성하는 함수
float4x4 CreateScaleMatrix(float3 scale)
{
	float4x4 scaleMatrix = float4x4(
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

	return scaleMatrix;
}

// 이동 값을 이용하여 이동 행렬을 생성하는 함수
float4x4 CreateTranslationMatrix(float3 translation)
{
	float4x4 translationMatrix = float4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translation.x, translation.y, translation.z, 1.0f
    );

	return translationMatrix;
}
// RST 값을 이용하여 행렬을 조립하는 함수
float4x4 AssembleMatrix(float3 rotation, float3 scale, float3 translation)
{
    float4x4 rotationMatrix = CreateRotationMatrix(rotation);
    float4x4 scaleMatrix = CreateScaleMatrix(scale);
    float4x4 translationMatrix = CreateTranslationMatrix(translation);

    // 회전 * 스케일 * 이동 순서로 행렬을 조립
    float4x4 finalMatrix = mul(translationMatrix, mul(scaleMatrix, rotationMatrix));

    return finalMatrix;
}
// 행렬에서 회전 부분을 추출하는 함수
float3 ExtractRotation(float4x4 input)
{
	return input[0].xyz;
}

// 행렬에서 스케일 부분을 추출하는 함수
float3 ExtractScale(float4x4 input)
{
	return input[1].xyz;
}

// 행렬에서 이동 부분을 추출하는 함수
float3 ExtractTranslation(float4x4 input)
{
	return input[3].xyz;
}

// 행렬에서 RST를 분리하는 함수
void DecomposeRST(float4x4 input, out float3 rotation, out float3 scale, out float3 translation)
{
	rotation = ExtractRotation(input);
	scale = ExtractScale(input);
	translation = ExtractTranslation(input);
}

// 각 구성 요소를 따로 보간하여 결과를 계산하는 함수
float4x4 InterpolateTransform(float4x4 start, float4x4 end, float t)
{
	float3 startTranslation;
	float3 endTranslation;

	float3 startRotation;
	float3 endRotation;

	float3 startScale;
	float3 endScale;

	DecomposeRST(start, startRotation, startScale, startTranslation);
	DecomposeRST(end, endRotation, endScale, endTranslation);
	
    // 각 구성 요소를 보간
	float3 interpolatedTranslation = lerp(startTranslation, endTranslation, t);
	float3 interpolatedRotation = lerp(startRotation, endRotation, t);
	float3 interpolatedScale = lerp(startScale, endScale, t);

    // 보간된 구성 요소를 결합하여 결과 행렬 생성
	float4x4 result = mul(mul(interpolatedScale, interpolatedRotation), interpolatedTranslation);

	return result;
}



// 'SkinWorld' 함수 정의: float4 형식의 'indices'와 'weights' 매개변수를 받음
matrix SkinWorld(float4 indices, float4 weights)
{
    // 결과 변환 행렬을 0으로 초기화
	matrix transform = 0;
	matrix nextTransform = 0;
    // 현재와 다음 애니메이션 프레임을 위한 행렬 선언
	matrix cur, next;
    
    // 시간 보간을 적용한 현재와 다음 애니메이션 프레임을 위한 행렬 선언
	matrix curAnim, nextAnim;
    
    // 현재와 다음 애니메이션 프레임의 구성 요소를 위한 float4 벡터 선언
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
    
    // 각 본에 대해 반복 (최적화를 위한 언롤 루프)
    [unroll(4)]
	for (int i = 0; i < 4; i++)
	{
        // 모션 데이터에서 현재 클립 및 프레임 정보를 가져옴
		int clip = motion.cur.clip;
		int curFrame = motion.cur.curFrame;
        
        // 텍스처에서 현재 애니메이션 프레임의 변환 행렬 구성 요소를 로드
		c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
		c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
		c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
		c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
        // 현재 애니메이션 프레임의 행렬 생성
		cur = matrix(c0, c1, c2, c3);
        
        // 다음 프레임의 변환 행렬 구성 요소 로드
		n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
		n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
		n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
		n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        
        // 다음 프레임의 행렬 생성
		next = matrix(n0, n1, n2, n3);
        
        // 현재와 다음 프레임을 시간에 따라 보간하여 현재 애니메이션 행렬 생성
		curAnim = InterpolateTransform(cur, next, motion.cur.time);
		transform += mul(weights[i], curAnim);
	}

	if (motion.next.clip <= -1)
		return transform;
        
	// 모션의 다음 클립 및 프레임 정보를 가져옴
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		int clip = motion.next.clip;
		int curFrame = motion.next.curFrame;
	
        // 클립이 유효한 경우에만 실행되는 조건문
        // 다시 다음 클립의 현재 및 다음 프레임의 변환 행렬 구성 요소 로드
		c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
		c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
		c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
		c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
        // 다시 현재 및 다음 프레임의 행렬 생성
		cur = matrix(c0, c1, c2, c3);
        
		n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
		n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
		n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
		n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        // 다음 프레임의 행렬 생성
		next = matrix(n0, n1, n2, n3);
           
        // 현재와 다음 애니메이션 행렬을 tweenTime에 따라 보간하여 최종 애니메이션 행렬 생성
		nextAnim = InterpolateTransform(cur, next, motion.next.time);
        // 가중치를 곱하여 결과 행렬에 더함
		nextTransform += mul(weights[i], nextAnim);
	}
    
    // 최종 결과 행렬 반환
	return InterpolateTransform(transform, nextTransform, motion.tweenTime);
}

