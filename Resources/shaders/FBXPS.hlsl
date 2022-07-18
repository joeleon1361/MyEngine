#include "FBX.hlsli"

static const float PI = 3.141592654f;

static float3 N;

static float3 s_baseColor;

static float s_metalness;

static float s_roughness;

Texture2D<float4> baseTex : register(t0);
Texture2D<float4> metalnessTex : register(t1);
Texture2D<float4> normalTex : register(t2);
Texture2D<float4> roughnessTex : register(t3);

SamplerState smp : register(s0);

// UE4のSmithモデル
float GeometricSmith(float cosine)
{
	float k = (s_roughness + 1.0f);

	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}

float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

// ディズニーのフレネル計算
float3 DisneyFresnel(float LdotH)
{
	// F項
	// 輝度
	float luminance = 0.3f * s_baseColor.r + 0.6f * s_baseColor.g + 0.1f * s_baseColor.b;

	// 色合い
	float3 tintColor = s_baseColor / luminance;

	// 非金属の鏡面反射を計算
	float3 nonMetalColor = specular * 0.08f * tintColor;

	// metalnessによる色補講
	float3 specularColor = lerp(nonMetalColor, baseColor, s_metalness);

	// NdotHの割合でSchlickFresnel補講
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

/// UE4のGGX分布
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

// 鏡面反射の計算
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	// D項
	float Ds = DistributionGGX(s_roughness * s_roughness, NdotH);

	// F項
	float3 Fs = DisneyFresnel(LdotH);

	// G項
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotL);

	// m項
	float m = 4.0f * NdotL * NdotV;

	// 合成して鏡面反射の色を得る
	return Ds * Fs * Gs / m;
}

// Schlickによる近似
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

	if (NdotL < 0 || NdotV < 0) { return float3(0, 0, 0); }

	// ライト方向とカメラ方向の中間
	float3 H = normalize(L + V);

	// 法線とハーフベクトルの内積
	float NdotH = dot(N, H);

	// ライトとハーフベクトルの内積
	float LdotH = dot(L, H);

	// 拡散反射率
	float diffuseReflectance = 1.0f / PI;

	float energyBias = 0.5f * s_roughness;

	// 入射角が90度の場合の拡散反射率
	float Fd90 = energyBias + 2 * LdotH * LdotH * s_roughness;

	// 入っていく時の拡散反射率
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);

	// 出ていく時の拡散反射率
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);

	float energyFactor = lerp(1.0f, 1.0f / 1.51f, s_roughness);

	// 入って出ていくまでの拡散反射率
	float Fd = FL * FV * energyFactor;

	// 拡散反射項
	float3 diffuseColor = diffuseReflectance * Fd * s_baseColor * (1 - s_metalness); // Schlick

	// 鏡面反射項
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);

	// 拡散反射と鏡面反射の合成
	return diffuseColor + specularColor;
}

float4 main(VSOutput input) : SV_TARGET
{
	N = input.normal;

	s_baseColor = baseColor + baseTex.Sample(smp, input.uv).rgb;
	s_metalness = metalness + metalnessTex.Sample(smp, input.uv).r;
	s_roughness = roughness + roughnessTex.Sample(smp, input.uv).r;

	float3 finalRGB = float3(0, 0, 0);

	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (!dirLights[i].active)
		{
			continue;
		}
		finalRGB += BRDF(dirLights[i].lightv, eyedir) * dirLights[i].lightcolor;
	}

	return float4(finalRGB, 1);
}