cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
};

struct VSInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 worldpos : POS;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

// ボーンの最大数
static const int MAX_BONES = 32;

cbuffer skinning:register(b3) // ボーンのスキニング行列が入る
{
	matrix matSkinning[MAX_BONES];
}

// マテリアル
cbuffer cbuff1 : register(b1)
{
	// アルベド
	float3 baseColor;

	// 金属度
	float metalness;

	// 鏡面反射強度
	float specular;

	// 粗さ
	float roughness;
}

// 平行光源の数
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;
	float3 lightcolor;
	uint active;
};

// 点光源の数
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

// スポットライトの数
static const int SPOTLIGHT_NUM = 3;

struct SpotLight
{
	float3 lightv;
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	float2 lightfactoranglecos;
	uint active;
};

cbuffer cbuff2 : register(b2)
{
	float3	ambientLightColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLight[SPOTLIGHT_NUM];
}