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
	// Ư�� ����, ���� �������� ��, � ���� ����ġ�� �� ������ ����Ʈ�� ��.
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

Texture2DArray transformMap : register(t0);

// ȸ�� ���� �̿��Ͽ� ȸ�� ����� �����ϴ� �Լ�
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

// ������ ���� �̿��Ͽ� ������ ����� �����ϴ� �Լ�
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

// �̵� ���� �̿��Ͽ� �̵� ����� �����ϴ� �Լ�
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
// RST ���� �̿��Ͽ� ����� �����ϴ� �Լ�
float4x4 AssembleMatrix(float3 rotation, float3 scale, float3 translation)
{
    float4x4 rotationMatrix = CreateRotationMatrix(rotation);
    float4x4 scaleMatrix = CreateScaleMatrix(scale);
    float4x4 translationMatrix = CreateTranslationMatrix(translation);

    // ȸ�� * ������ * �̵� ������ ����� ����
    float4x4 finalMatrix = mul(translationMatrix, mul(scaleMatrix, rotationMatrix));

    return finalMatrix;
}
// ��Ŀ��� ȸ�� �κ��� �����ϴ� �Լ�
float3 ExtractRotation(float4x4 input)
{
	return input[0].xyz;
}

// ��Ŀ��� ������ �κ��� �����ϴ� �Լ�
float3 ExtractScale(float4x4 input)
{
	return input[1].xyz;
}

// ��Ŀ��� �̵� �κ��� �����ϴ� �Լ�
float3 ExtractTranslation(float4x4 input)
{
	return input[3].xyz;
}

// ��Ŀ��� RST�� �и��ϴ� �Լ�
void DecomposeRST(float4x4 input, out float3 rotation, out float3 scale, out float3 translation)
{
	rotation = ExtractRotation(input);
	scale = ExtractScale(input);
	translation = ExtractTranslation(input);
}

// �� ���� ��Ҹ� ���� �����Ͽ� ����� ����ϴ� �Լ�
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
	
    // �� ���� ��Ҹ� ����
	float3 interpolatedTranslation = lerp(startTranslation, endTranslation, t);
	float3 interpolatedRotation = lerp(startRotation, endRotation, t);
	float3 interpolatedScale = lerp(startScale, endScale, t);

    // ������ ���� ��Ҹ� �����Ͽ� ��� ��� ����
	float4x4 result = mul(mul(interpolatedScale, interpolatedRotation), interpolatedTranslation);

	return result;
}



// 'SkinWorld' �Լ� ����: float4 ������ 'indices'�� 'weights' �Ű������� ����
matrix SkinWorld(float4 indices, float4 weights)
{
    // ��� ��ȯ ����� 0���� �ʱ�ȭ
	matrix transform = 0;
	matrix nextTransform = 0;
    // ����� ���� �ִϸ��̼� �������� ���� ��� ����
	matrix cur, next;
    
    // �ð� ������ ������ ����� ���� �ִϸ��̼� �������� ���� ��� ����
	matrix curAnim, nextAnim;
    
    // ����� ���� �ִϸ��̼� �������� ���� ��Ҹ� ���� float4 ���� ����
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
    
    // �� ���� ���� �ݺ� (����ȭ�� ���� ��� ����)
    [unroll(4)]
	for (int i = 0; i < 4; i++)
	{
        // ��� �����Ϳ��� ���� Ŭ�� �� ������ ������ ������
		int clip = motion.cur.clip;
		int curFrame = motion.cur.curFrame;
        
        // �ؽ�ó���� ���� �ִϸ��̼� �������� ��ȯ ��� ���� ��Ҹ� �ε�
		c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
		c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
		c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
		c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
        // ���� �ִϸ��̼� �������� ��� ����
		cur = matrix(c0, c1, c2, c3);
        
        // ���� �������� ��ȯ ��� ���� ��� �ε�
		n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
		n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
		n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
		n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        
        // ���� �������� ��� ����
		next = matrix(n0, n1, n2, n3);
        
        // ����� ���� �������� �ð��� ���� �����Ͽ� ���� �ִϸ��̼� ��� ����
		curAnim = InterpolateTransform(cur, next, motion.cur.time);
		transform += mul(weights[i], curAnim);
	}

	if (motion.next.clip <= -1)
		return transform;
        
	// ����� ���� Ŭ�� �� ������ ������ ������
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		int clip = motion.next.clip;
		int curFrame = motion.next.curFrame;
	
        // Ŭ���� ��ȿ�� ��쿡�� ����Ǵ� ���ǹ�
        // �ٽ� ���� Ŭ���� ���� �� ���� �������� ��ȯ ��� ���� ��� �ε�
		c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
		c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
		c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
		c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
        // �ٽ� ���� �� ���� �������� ��� ����
		cur = matrix(c0, c1, c2, c3);
        
		n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
		n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
		n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
		n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        // ���� �������� ��� ����
		next = matrix(n0, n1, n2, n3);
           
        // ����� ���� �ִϸ��̼� ����� tweenTime�� ���� �����Ͽ� ���� �ִϸ��̼� ��� ����
		nextAnim = InterpolateTransform(cur, next, motion.next.time);
        // ����ġ�� ���Ͽ� ��� ��Ŀ� ����
		nextTransform += mul(weights[i], nextAnim);
	}
    
    // ���� ��� ��� ��ȯ
	return InterpolateTransform(transform, nextTransform, motion.tweenTime);
}

