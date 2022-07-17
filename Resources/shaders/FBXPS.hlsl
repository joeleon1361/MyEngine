#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

static const float PI = 3.141592654f;

static float3 N;

float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}



float3 BRDF(float3 L, float3 V)
{
	float NdotL = dot(N, L);

	float NdotV = dot(N, V);

	if (NdotL < 0 || NdotV < 0)
	{
		return float3(0, 0, 0);
	}

	float3 H = normalize(L + V);

	float NdotH = dot(N, H);

	float LdotH = dot(L, H);

	float diffuseReflectance = 1.0f / PI;

	float energyBias = 0.5f * roughness;
	float Fd90 = energyBias + 2 * LdotH * LdotH * roughness;

	float FL = SchlickFresnel(1.0f, Fd90, NdotL);

	float FV = SchlickFresnel(1.0f, Fd90, NdotV);

	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
	float Fd = FL * FV * energyFactor;

	float3 diffuseColor = diffuseReflectance * Fd * baseColor * (1 - metalness);

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

