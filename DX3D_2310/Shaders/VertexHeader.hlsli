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

cbuffer FrameBuffer : register(b3)
{
	int clip;
	int curFrame;
	int nextcurFrame;
	float time;
}

Texture2DArray transformMap : register(t0);

matrix SkinWorld(float4 indices, float4 weights)
{
	matrix transform = 0;
	matrix cur;
	matrix nextcur;
	
	matrix curanim;
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	// 매트릭스 뽑는것.
	[unroll(4)] // 제한하는 구문. 4개이상 절때 돌아갈 수 없게 제한한다.
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
		//curanim = cur;
		transform += mul(weights[i], curanim);
	}
	
	return transform;
}