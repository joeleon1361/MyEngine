#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "FbxModel.h"
#include "Camera.h"

class FbxObject3d
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 定数
// ボーンの最大数
	static const int MAX_BONES = 32;

public: // サブクラス
// 定数バッファ用データ構造体(座標変換行列用)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	// 定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};


public: // 静的メンバ関数
// setter
	static void SetDevice( ID3D12Device *device ) { FbxObject3d::device = device; }
	static void SetCamera( Camera *camera ) { FbxObject3d::camera = camera; }

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

private: // 静的メンバ変数
	static ID3D12Device *device;

	// カメラ
	static Camera *camera;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;

	// 定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;

public: // メンバ関数
	// 初期化
	void Initialize();

	// 毎フレーム処理
	void Update();
	
	// モデルのセット
	void SetModel( FbxModel *fbxmodel ) { this->fbxmodel = fbxmodel; }
	
	// 描画
	void Draw( ID3D12GraphicsCommandList *cmdList );
	
	// 座標の取得
	const XMFLOAT3 &GetPosition() { return position; }
	
	// 座標の設定
	void SetPosition( XMFLOAT3 position ) { this->position = position; }

	void SetRotation( XMFLOAT3 rotation ) { this->rotation = rotation; }
	
	// スケールの設定
	void SetScale( XMFLOAT3 scale ) { this->scale = scale; }
	
	// アニメーション開始
	void PlayAnimation();

protected: // メンバ変数
// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// モデル
	FbxModel *fbxmodel = nullptr;

	// 1フレーム
	FbxTime frameTime;
	// アニメーション開始時間
	FbxTime startTime;
	// アニメーション終了時間
	FbxTime endTime;
	// 現在時間
	FbxTime currentTime;
	// アニメーション再生中
	bool isPlay = false;
};

