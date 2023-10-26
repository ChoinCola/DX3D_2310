//PixelHeader.hlsli

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

cbuffer LightBuffer : register(b0)
{
	// Á÷Á¢±¤
	float3 lightDirection;
	// ºûÀÇ ¹İ»çÁ¤µµ
	float badding;
	// °£Á¢±¤
	float4 ambientLight;
}

cbuffer MaterialBuffer : register(b2)
{
	// ÀçÁú
	float4 mDiffuse;
	float4 mSpecular;
	float4 mAmbient;
	float4 mEmissive;
	
	float shininess;
	int hasNormalMap;
}