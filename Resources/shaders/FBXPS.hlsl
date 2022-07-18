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

// UE4��Smith���f��
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

// �f�B�Y�j�[�̃t���l���v�Z
float3 DisneyFresnel(float LdotH)
{
	// F��
	// �P�x
	float luminance = 0.3f * s_baseColor.r + 0.6f * s_baseColor.g + 0.1f * s_baseColor.b;

	// �F����
	float3 tintColor = s_baseColor / luminance;

	// ������̋��ʔ��˂��v�Z
	float3 nonMetalColor = specular * 0.08f * tintColor;

	// metalness�ɂ��F��u
	float3 specularColor = lerp(nonMetalColor, baseColor, s_metalness);

	// NdotH�̊�����SchlickFresnel��u
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

/// UE4��GGX���z
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

// ���ʔ��˂̌v�Z
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	// D��
	float Ds = DistributionGGX(s_roughness * s_roughness, NdotH);

	// F��
	float3 Fs = DisneyFresnel(LdotH);

	// G��
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotL);

	// m��
	float m = 4.0f * NdotL * NdotV;

	// �������ċ��ʔ��˂̐F�𓾂�
	return Ds * Fs * Gs / m;
}

// Schlick�ɂ��ߎ�
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

	// ���C�g�����ƃJ���������̒���
	float3 H = normalize(L + V);

	// �@���ƃn�[�t�x�N�g���̓���
	float NdotH = dot(N, H);

	// ���C�g�ƃn�[�t�x�N�g���̓���
	float LdotH = dot(L, H);

	// �g�U���˗�
	float diffuseReflectance = 1.0f / PI;

	float energyBias = 0.5f * s_roughness;

	// ���ˊp��90�x�̏ꍇ�̊g�U���˗�
	float Fd90 = energyBias + 2 * LdotH * LdotH * s_roughness;

	// �����Ă������̊g�U���˗�
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);

	// �o�Ă������̊g�U���˗�
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);

	float energyFactor = lerp(1.0f, 1.0f / 1.51f, s_roughness);

	// �����ďo�Ă����܂ł̊g�U���˗�
	float Fd = FL * FV * energyFactor;

	// �g�U���ˍ�
	float3 diffuseColor = diffuseReflectance * Fd * s_baseColor * (1 - s_metalness); // Schlick

	// ���ʔ��ˍ�
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);

	// �g�U���˂Ƌ��ʔ��˂̍���
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