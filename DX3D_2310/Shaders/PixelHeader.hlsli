//PixelHeader.hlsli

Texture2D diffuseMap : register(t0);
SamplerState samp : register(s0);

cbuffer LightBuffer : register(b0)
{
	float3 lightDirection;
	float shininess;
}
