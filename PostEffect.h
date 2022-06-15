#pragma once
#include "Sprite.h"

class PostEffect :
	public Sprite
{
public: // �ÓI�����o�֐�
	 // �V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	// �V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private: // �ÓI�����o�ϐ�
	// ��ʃN���A�J���[
	static const float clearColor[4];

public: // �����o�֐�
	// �R���X�g���N�^
	PostEffect();

	// ������
	void Initialize();

	// �`��R�}���h�̔��s
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;

	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};
