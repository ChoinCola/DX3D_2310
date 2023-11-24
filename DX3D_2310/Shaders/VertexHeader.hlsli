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

// 입력된 회전값을 쿼터니언값으로 반환하는 함수.
float4 ExtractRotationtoQutanion(float3x3 input)
{
	float4 result;
	
	// 구성요소의 크기를 구한다. 가장 큰 크기를 찾아야함.
	result.w = abs(sqrt((1 + input._11 + input._22 + input._33) / 4)); // w
	result.x = abs(sqrt((1 + input._11 - input._22 - input._33) / 4)); // x
	result.y = abs(sqrt((1 - input._11 + input._22 - input._33) / 4)); // y
	result.z = abs(sqrt((1 - input._11 - input._22 + input._33) / 4)); // z
	
	float max1 = max(result.w, result.x);
	float max2 = max(result.y, result.z);
	float finalMax = max(max1, max2);
	
	// 가장 큰 크기에 따라 부호가 달라짐.
	if(finalMax == result.w)
	{
		result.x = (input._32 - input._23) / (4 * result.w);
		result.y = (input._13 - input._31) / (4 * result.w);
		result.z = (input._21 - input._12) / (4 * result.w);
		result = normalize(result);

		return result;
	}
	else if (finalMax == result.x)
	{
		result.w = (input._32 - input._23) / (4 * result.x);
		result.y = (input._12 + input._21) / (4 * result.x);
		result.z = (input._13 + input._31) / (4 * result.x);
		result = normalize(result);

		return result;
	}
	else if (finalMax == result.y)
	{
		result.w = (input._13 - input._31) / (4 * result.y);
		result.x = (input._12 + input._21) / (4 * result.y);
		result.z = (input._23 + input._32) / (4 * result.y);
		result = normalize(result);

		return result;
	}
	else
	{
		result.w = (input._21 - input._12) / (4 * result.z);
		result.x = (input._13 + input._31) / (4 * result.z);
		result.y = (input._23 + input._32) / (4 * result.z);
		result = normalize(result);

		return result;
	}
}
// 쿼터니언 구면 선형보간(selrp)
float4 slerp(float4 quA, float4 quB, float time)
{
	
// 두 쿼터니언 간의 내적을 계산합니다.
	float dotProduct = dot(quA, quB);

// 두 쿼터니언 간의 각도(Theta)를 계산합니다. 아크코사인을 사용하여 계산됩니다.
	float theta = acos(dotProduct);

// 각도에 대한 사인 값을 계산합니다. 이 값은 구면 보간에서 사용됩니다.
	float sinTheta = sin(theta);

// 결과로 사용할 쿼터니언을 초기화합니다.
	float4 result;

// SLERP 보간을 수행합니다.
    // 각 쿼터니언의 각 성분을 구면 보간합니다.
	result = (sin((1 - time) * theta) / sinTheta) * quA + (sin(time * theta) / sinTheta) * quB;
    // 보간된 쿼터니언을 정규화합니다.
	result = normalize(result);

    // 보간된 정규화된 쿼터니언을 반환합니다.
	return result;
}

float3x3 QuaternionToMatrix(float4 qu)
{
    // 쿼터니언은 노멀라이즈 된 값으로 한다. w = 1
	qu = normalize(qu);

    // 데이터를 사전정리
	float xx = qu.x * qu.x;
	float yy = qu.y * qu.y;
	float zz = qu.z * qu.z;
	float xy = qu.x * qu.y;
	float xz = qu.x * qu.z;
	float yz = qu.y * qu.z;
	float wx = qu.w * qu.x;
	float wy = qu.w * qu.y;
	float wz = qu.w * qu.z;

    // 메트릭스로 반환.
	float3x3 result =
    (1 - 2 * (yy + zz), 2 * (xy - wz), 2 * (xz + wy),
         2 * (xy + wz), 1 - 2 * (xx + zz), 2 * (yz - wx),
         2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (xx + yy));

	return result;
}

// 행렬에서 이동 부분을 추출하는 함수
float3 ExtractTranslation(float4x4 input)
{
	float3 output = input._41_42_43;
	
	return output;
}
// 입력된 데이터에서 스케일값을 뽑아내는 함수.
float3 ExtractScale(float4x4 input)
{
	float3 output;

	output.x = length(input._11_12_13);
	output.y = length(input._21_22_23);
	output.z = length(input._31_32_33);
	
	return output;
}
// 입력된 데이터에서 로테이션 값을 뽑아내는 함수.
float3x3 ExtractRotation(float4x4 input)
{
	input._41_42_43 = 0;
	
	float3 scale = ExtractScale(input);
	scale = 1 / scale;
	float4x4 ScaleInverseMatrix = 0;
	ScaleInverseMatrix._11_22_33 = scale.xyz;
	ScaleInverseMatrix._44 = 1;
	
	input = mul(input, ScaleInverseMatrix);
	
	float3x3 result;
	result._11 = input._11;
	result._12 = input._12;
	result._13 = input._13;
	result._21 = input._21;
	result._22 = input._22;
	result._23 = input._23;
	result._31 = input._31;
	result._32 = input._32;
	result._33 = input._33;
	
	return result;
}

// 행렬에서 SRT를 분리하는 함수
void DecomposeRST(float4x4 input, out float3x3 rotation, out float3 scale, out float3 translation)
{
	translation = ExtractTranslation(input);
	rotation = ExtractRotation(input);
	scale = ExtractScale(input);
}


// SRT를 행렬로 변환하는 함수
float4x4 SRTtoMatrix(float3 scale, float3x3 rotation, float3 translation)
{
	float4x4 result;
	result._11_22_33_44 = 1;
	
	
	float4x4 Mscale = 0;
	float4x4 Mrotation = 0;
	float4x4 Mtranslation = 0;
	
	Mscale._11_22_33_44 = 1;
	Mrotation._11_22_33_44 = 1;
	Mtranslation._11_22_33_44 = 1;
	
	Mscale._11_22_33 = scale.xyz;
	
	Mrotation._11_12_13 = rotation._11_12_13;
	Mrotation._21_22_23 = rotation._21_22_23;
	Mrotation._31_32_33 = rotation._31_32_33;
	
	Mtranslation._41_42_43 = translation.xyz;
	
	result = mul(mul(Mscale, Mrotation), Mtranslation);
	
	return result;
}


// 각 구성 요소를 따로 보간하여 결과를 계산하는 함수
float4x4 InterpolateTransform(float4x4 start, float4x4 end, float t)
{
	float3 startTranslation;
	float3 endTranslation;

	float3x3 startRotation;
	float3x3 endRotation;

	float3 startScale;
	float3 endScale;

	// 각 데이터를 추출.
	DecomposeRST(start, startRotation, startScale, startTranslation);
	DecomposeRST(end, endRotation, endScale, endTranslation);
	
	
	// 로테이션 값은 쿼터니언 데이터에서 선형보간 작업을 진행해야 한다.
	// 쿼터니언 변환
	float4 startqu = ExtractRotationtoQutanion(startRotation);
	float4 endqu = ExtractRotationtoQutanion(endRotation);
	
	// 쿼터니언 보간
	float4 interpolatedQutonion = slerp(startqu, endqu, t);
	
    // 각 구성 요소를 보간
	float3 interpolatedTranslation = lerp(startTranslation, endTranslation, t);
	float3 interpolatedScale = lerp(startScale, endScale, t);
	
	// 로테이션은 쿼터니언값을 3x3 로테이션 행렬로 변환한다.
	float3x3 interpolatedRotation = QuaternionToMatrix(interpolatedQutonion);
	//float3x3 interpolatedRotation = lerp(startRotation, endRotation, t);
	
    // 보간된 구성 요소를 결합하여 결과 행렬 생성
	//float4x4 result = mul(mul(interpolatedScale, interpolatedRotation), interpolatedTranslation);

	return SRTtoMatrix(interpolatedScale, interpolatedRotation, interpolatedTranslation);
}

float4x4 MakeMatrix(float4 Scale, float4 Rotation, float4 Translation)
{
	float4x4 scale = 0;
	scale._11_22_33_44 = 1;
	float4x4 rotation = 0;
	rotation._11_22_33_44 = 1;
	float4x4 translation = 0;
	translation._11_22_33_44 = 1;
	
	scale._11_22_33 = Scale.xyz;
	translation._41_42_43 = Translation.xyz;
	
	float3x3 qutoromat;
	
	qutoromat = QuaternionToMatrix(Rotation);
	
	rotation._11_21_31 = qutoromat._11_21_31;
	rotation._12_22_32 = qutoromat._12_22_32;
	rotation._13_23_33 = qutoromat._13_23_33;

	return mul(mul(translation, rotation), scale);
}


// 'SkinWorld' 함수 정의: float4 형식의 'indices'와 'weights' 매개변수를 받음
matrix SkinWorld(float4 indices, float4 weights)
{
	matrix transform = 0;
	matrix cur, next;
	
	matrix curAnim, nextAnim;
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		int clip = motion.cur.clip;
		int curFrame = motion.cur.curFrame;
		
		c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
		c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
		c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
		c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
		
		cur = matrix(c0, c1, c2, c3);
		
		n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
		n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
		n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
		n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
		
		next = matrix(n0, n1, n2, n3);
		
		curAnim = lerp(cur, next, motion.cur.time);
		
		clip = motion.next.clip;
		curFrame = motion.next.curFrame;
		
		[flatten]
		if (clip > -1)
		{
			c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
			c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
			c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
			c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
		
			cur = matrix(c0, c1, c2, c3);
		
			n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
			n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
			n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
			n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
		
			next = matrix(n0, n1, n2, n3);
		
			nextAnim = lerp(cur, next, motion.next.time);
			
			curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
		}
		
		transform += mul(weights[i], curAnim);
	}
	
	return transform;
}