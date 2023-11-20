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

cbuffer FrameBuffer : register(b3)
{
	int clip;
	int nextclip;
	int curFrame;
	int nextcurFrame;
	float time;
	float scale; // �ִϸ��̼� ��� �ӵ� ������ ������
	float transtime; // �ִϸ��̼� ��ȯ�ӵ� ������ ������
	float transtimemax; // ��ȯ�� �ִ밪

}

Texture2DArray transformMap : register(t0);

matrix SkinWorld(float4 indices, float4 weights)
{
	matrix resulttransform = 0;
	
	matrix transform = 0;
	matrix nexttransform = 0;
	matrix cur;
	matrix nextcur;
	
	matrix curanim;
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	// ��Ʈ���� �̴°�.
	[unroll(4)] // �����ϴ� ����. 4���̻� ���� ���ư� �� ���� �����Ѵ�.
	for (int i = 0; i < 4; i++)
	{
		c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
		c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
		c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
		c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
		
		cur = matrix(c0, c1, c2, c3);
		
		n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextcurFrame, clip, 0));
		n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextcurFrame, clip, 0));
		n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextcurFrame, clip, 0));
		n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextcurFrame, clip, 0));
		
		nextcur = matrix(n0, n1, n2, n3);
	
		curanim = lerp(cur, nextcur, time);
		transform += mul(weights[i], curanim);
	}
	// transtime�� 0�̶��, ���� transform�� �� �ʿ䰡 ���� ����,
	if (transtime <= 0.00001)
		return transform;

	// �ٲ�� ���� ����� ���������� ����
	// nextFrmae
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, nextclip, 0));
		c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, nextclip, 0));
		c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, nextclip, 0));
		c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, nextclip, 0));
		
		cur = matrix(c0, c1, c2, c3);
		
		n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextcurFrame, nextclip, 0));
		n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextcurFrame, nextclip, 0));
		n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextcurFrame, nextclip, 0));
		n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextcurFrame, nextclip, 0));
		
		nextcur = matrix(n0, n1, n2, n3);

		curanim = lerp(cur, nextcur, time);
		nexttransform += mul(weights[i], curanim);
	}
	
	resulttransform = lerp(transform, nexttransform, transtime / transtimemax);
	return resulttransform;
}