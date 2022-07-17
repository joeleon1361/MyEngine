#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

static const float PI = 3.141592654f;

static float3 N;

float3 BRDF(float3 L, float3 V)
{
	float NdotL = dot(N, L);

	float NdotV = dot(N, V);

	if (NdotL < 0 || NdotV < 0)
	{
		return float3(0, 0, 0);
	}

	float diffuseReflectance = 1.0f / PI;

	float3 diffuseColor = diffuseReflectance * NdotL * baseColor * (1 - metalness);

	return diffuseColor;
}

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

float4 main(VSOutput input) : SV_TARGET
{
	N = input.normal;

	float3 finalRGB = float3(0, 0, 0);

	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (!dirLights[i].active) {
			continue;
		}
		finalRGB += BRDF(dirLights[i].lightv, eyedir) * dirLights[i].lightcolor;
	}
	return float4(finalRGB, 1);
}