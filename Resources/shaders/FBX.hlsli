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

// �{�[���̍ő吔
static const int MAX_BONES = 32;

cbuffer skinning:register(b3) // �{�[���̃X�L�j���O�s�񂪓���
{
	matrix matSkinning[MAX_BONES];
}

// �}�e���A��
cbuffer cbuff1 : register(b1)
{
	// �A���x�h
	float3 baseColor;

	// �����x
	float metalness;

	// ���ʔ��ˋ��x
	float specular;

	// �e��
	float roughness;
}

// ���s�����̐�
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;
	float3 lightcolor;
	uint active;
};

// �_�����̐�
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

// �X�|�b�g���C�g�̐�
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