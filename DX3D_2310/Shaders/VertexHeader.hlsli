// VertexHeader.hlsli
#define MAX_INSTANCE 256

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
	
	matrix transform : INSTANCE_TRANSFORM;
};

struct VertexUVNormalTangentBlend
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 indices : BLENDINDICES;
	// Ư�� ����, ���� �������� ��, � ���� ����ġ�� �� ������ ����Ʈ�� ��.
	float4 weights : BLENDWEIGHTS;
	
	matrix transform : INSTANCE_TRANSFORM;
};

struct VertexInstancing
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 indices : BLENDINDICES;
	// Ư�� ����, ���� �������� ��, � ���� ����ġ�� �� ������ ����Ʈ�� ��.
	float4 weights : BLENDWEIGHTS;
	
	matrix transform : INSTANCE_TRANSFORM;
	int index : INSTANCE_INDEX;
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
	float scale; // �ִϸ��̼� ��� �ӵ� ������ ������
};

struct Motion
{
	float takeTime; // �����ð�
	float tweenTime; // ��� �ð�
	float running_time;
	float padding;
	
	Frame cur, next;
};

cbuffer FrameBuffer : register(b3)
{
	Motion motion;
}

cbuffer FrameInstancingBuffer : register(b4)
{
	Motion motions[MAX_INSTANCE];
}
Texture2DArray transformMap : register(t0);

// �Էµ� ȸ������ ���ʹϾ����� ��ȯ�ϴ� �Լ�.
float4 ExtractRotationtoQutanion(float3x3 input)
{
	float4 result;
	
	// ��������� ũ�⸦ ���Ѵ�. ���� ū ũ�⸦ ã�ƾ���.
	result.w = abs(sqrt((1 + input._11 + input._22 + input._33) / 4)); // w
	result.x = abs(sqrt((1 + input._11 - input._22 - input._33) / 4)); // x
	result.y = abs(sqrt((1 - input._11 + input._22 - input._33) / 4)); // y
	result.z = abs(sqrt((1 - input._11 - input._22 + input._33) / 4)); // z
	
	float max1 = max(result.w, result.x);
	float max2 = max(result.y, result.z);
	float finalMax = max(max1, max2);
	
	// ���� ū ũ�⿡ ���� ��ȣ�� �޶���.
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
// ���ʹϾ� ���� ��������(selrp)
float4 slerp(float4 quA, float4 quB, float time)
{
	
// �� ���ʹϾ� ���� ������ ����մϴ�.
	float dotProduct = dot(quA, quB);

// �� ���ʹϾ� ���� ����(Theta)�� ����մϴ�. ��ũ�ڻ����� ����Ͽ� ���˴ϴ�.
	float theta = acos(dotProduct);

// ������ ���� ���� ���� ����մϴ�. �� ���� ���� �������� ���˴ϴ�.
	float sinTheta = sin(theta);

// ����� ����� ���ʹϾ��� �ʱ�ȭ�մϴ�.
	float4 result;

// SLERP ������ �����մϴ�.
    // �� ���ʹϾ��� �� ������ ���� �����մϴ�.
	result = (sin((1 - time) * theta) / sinTheta) * quA + (sin(time * theta) / sinTheta) * quB;
    // ������ ���ʹϾ��� ����ȭ�մϴ�.
	result = normalize(result);

    // ������ ����ȭ�� ���ʹϾ��� ��ȯ�մϴ�.
	return result;
}

float3x3 QuaternionToMatrix(float4 qu)
{
    // ���ʹϾ��� ��ֶ����� �� ������ �Ѵ�. w = 1
	qu = normalize(qu);

    // �����͸� ��������
	float xx = qu.x * qu.x;
	float yy = qu.y * qu.y;
	float zz = qu.z * qu.z;
	float xy = qu.x * qu.y;
	float xz = qu.x * qu.z;
	float yz = qu.y * qu.z;
	float wx = qu.w * qu.x;
	float wy = qu.w * qu.y;
	float wz = qu.w * qu.z;

    // ��Ʈ������ ��ȯ.
	float3x3 result =
    (1 - 2 * (yy + zz), 2 * (xy - wz), 2 * (xz + wy),
         2 * (xy + wz), 1 - 2 * (xx + zz), 2 * (yz - wx),
         2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (xx + yy));

	return result;
}

// ��Ŀ��� �̵� �κ��� �����ϴ� �Լ�
float3 ExtractTranslation(float4x4 input)
{
	float3 output = input._41_42_43;
	
	return output;
}
// �Էµ� �����Ϳ��� �����ϰ��� �̾Ƴ��� �Լ�.
float3 ExtractScale(float4x4 input)
{
	float3 output;

	output.x = length(input._11_12_13);
	output.y = length(input._21_22_23);
	output.z = length(input._31_32_33);
	
	return output;
}
// �Էµ� �����Ϳ��� �����̼� ���� �̾Ƴ��� �Լ�.
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

// ��Ŀ��� SRT�� �и��ϴ� �Լ�
void DecomposeRST(float4x4 input, out float3x3 rotation, out float3 scale, out float3 translation)
{
	translation = ExtractTranslation(input);
	rotation = ExtractRotation(input);
	scale = ExtractScale(input);
}


// SRT�� ��ķ� ��ȯ�ϴ� �Լ�
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


// �� ���� ��Ҹ� ���� �����Ͽ� ����� ����ϴ� �Լ�
float4x4 InterpolateTransform(float4x4 start, float4x4 end, float t)
{
	float3 startTranslation;
	float3 endTranslation;

	float3x3 startRotation;
	float3x3 endRotation;

	float3 startScale;
	float3 endScale;

	// �� �����͸� ����.
	DecomposeRST(start, startRotation, startScale, startTranslation);
	DecomposeRST(end, endRotation, endScale, endTranslation);
	
	
	// �����̼� ���� ���ʹϾ� �����Ϳ��� �������� �۾��� �����ؾ� �Ѵ�.
	// ���ʹϾ� ��ȯ
	float4 startqu = ExtractRotationtoQutanion(startRotation);
	float4 endqu = ExtractRotationtoQutanion(endRotation);
	
	// ���ʹϾ� ����
	float4 interpolatedQutonion = slerp(startqu, endqu, t);
	
    // �� ���� ��Ҹ� ����
	float3 interpolatedTranslation = lerp(startTranslation, endTranslation, t);
	float3 interpolatedScale = lerp(startScale, endScale, t);
	
	// �����̼��� ���ʹϾ��� 3x3 �����̼� ��ķ� ��ȯ�Ѵ�.
	float3x3 interpolatedRotation = QuaternionToMatrix(interpolatedQutonion);
	//float3x3 interpolatedRotation = lerp(startRotation, endRotation, t);
	
    // ������ ���� ��Ҹ� �����Ͽ� ��� ��� ����
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

matrix SkinWorld(int instanceiIndex, float4 indices, float4 weights)
{
	Motion motion = motions[instanceiIndex];
	
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

// 'SkinWorld' �Լ� ����: float4 ������ 'indices'�� 'weights' �Ű������� ����
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