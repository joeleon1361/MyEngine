#pragma once

#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

// �m�[�h�N���X
struct Node
{
	// ���O
	std::string name;

	// ���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };

	// ���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };

	// ���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };

	// ���[�J���ό`�s��
	DirectX::XMMATRIX transform;

	// �O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;

	// �e�m�[�h
	Node* parent = nullptr;
};

// �e�N�X�`���f�[�^
struct TextureData
{
	// �e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};

	// �X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};

	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;

	// SRV��GPU�n���h��
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

class FbxModel
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	// std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public: // �萔
	// �{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

	// �e�N�X�`���̍ő吔
	static const int MAX_TEXTURES = 4;

public: // �T�u�N���X
// ���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv; // uv���W
		UINT boneIndex[MAX_BONE_INDICES]; // �{�[�� �ԍ�
		float boneWeight[MAX_BONE_INDICES]; // �{�[�� �d��
	};

public:
	// �t�����h�N���X
	friend class FbxLoader;

	struct Bone
	{
		// ���O
		std::string name;

		// �����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;

		// �N���X�^�[(FBX���̃{�[�����)
		FbxCluster* fbxCluster;

		// �R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	// �萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial
	{
		// �A���x�h
		DirectX::XMFLOAT3 baseColor;

		// �����x
		float metalness;

		// ���ʔ��ˋ��x
		float specular;

		// �e��
		float roughness;

		// �p�f�B���O
		float pad[2];
	};

public:
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void CreateTexture(TextureData& texture, ID3D12Device* device, int srvIndex);

private: // �����o�ϐ�
	// ���f����
	std::string name;

	// �m�[�h�z��
	std::vector<Node> nodes;
	// ���b�V�������m�[�h
	Node* meshNode = nullptr;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	// ComPtr<ID3D12Resource> texBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	// �A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	// �f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	// �e�N�X�`�����^�f�[�^
	// DirectX::TexMetadata metadata = {};
	// �X�N���b�`�C���[�W
	// DirectX::ScratchImage scratchImg = {};

	// �x�[�X�e�N�X�`��
	TextureData baseTexture;

	// ���^���l�X�e�N�X�`��
	TextureData metalnessTexture;

	// �@���e�N�X�`��
	TextureData normalTexture;

	// ���t�l�X�e�N�X�`��
	TextureData roughnessTexture;

	// �{�[���z��
	std::vector<Bone> bones;

	// FBX�V�[��
	FbxScene* fbxScene = nullptr;

	// �A���x�h
	DirectX::XMFLOAT3 baseColor = { 1,1,1 };

	// �����x(0 or 1)
	float metalness = 0.0f;

	// ���ʔ��˓x(0 �` 1)
	float specular = 0.5f;

	// �e��
	float roughness = 0.0f;

	// �萔�o�b�t�@(�}�e���A��)
	ComPtr<ID3D12Resource> constBuffMaterial;



public:
	// �o�b�t�@����
	void CreatBuffers(ID3D12Device* device);

	// ���f���̕ό`�s��擾
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	// getter
	std::vector<Bone>& GetBones() { return bones; }

	// getter
	FbxScene* GetFbxScene() { return fbxScene; }

	// �f�X�g���N�^
	~FbxModel();

	// getter
	const DirectX::XMFLOAT3& GetBaseColor() { return baseColor; }
	float GetMetalness() { return metalness; }
	float GetSpecular() { return specular; }
	float GetRoughness() { return roughness; }

	// setter
	void SetBaseColor(const DirectX::XMFLOAT3& _baseColor)
	{
		baseColor = _baseColor;
	}

	void SetMetalness(float _metalness)
	{
		metalness = _metalness;
	}

	void SetSpecular(float _specular)
	{
		specular = _specular;
	}

	void SetRoughness(float _roughness)
	{
		roughness = _roughness;
	}

	// �}�e���A���p�����[�^�]��
	void TransferMaterial();
};

