#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

float4 main(VSOutput input) : SV_TARGET
{
	return float4(baseColor, 1);
}