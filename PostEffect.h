#pragma once
#include "Sprite.h"

class PostEffect :
	public Sprite
{
public: // 静的メンバ関数
	 // シーン描画前処理
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	// シーン描画後処理
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private: // 静的メンバ変数
	// 画面クリアカラー
	static const float clearColor[4];

public: // メンバ関数
	// コンストラクタ
	PostEffect();

	// 初期化
	void Initialize();

	// パイプライン生成
	void CreateGraphicsPipelineState();

	// 描画コマンドの発行
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelineState;

	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;

	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff;

	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};

