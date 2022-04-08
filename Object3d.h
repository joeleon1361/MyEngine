#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "Model.h"
#include "Camera.h"

// 3Dオブジェクト
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス	

	// パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// ３Ｄ変換行列
	};

private: // 定数


public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize( ID3D12Device *device, Camera *camera = nullptr);

	// グラフィックパイプラインの生成
	static void CreateGraphicsPipeline();
	
	// カメラのセット
	static void SetCamera( Camera *camera ) {
		Object3d::camera = camera;
	}
	
	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	
	// 描画後処理
	static void PostDraw();

	// 3Dオブジェクト生成
	static Object3d* Create( Model *model = nullptr );


private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// テクスチャあり用パイプライン
	static PipelineSet pipelineSet;
	// カメラ
	static Camera *camera;

private:// 静的メンバ関数
	
public: // メンバ関数
	bool Initialize();
	// 毎フレーム処理
	void Update();

	// 描画
	void Draw();

	// 座標の取得
	const XMFLOAT3& GetPosition() { return position; }

	// 座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	// 角度の設定
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	// スケールの設定
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	// モデルのセット
	void SetModel(Model* model) { this->model = model; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
	// モデル
	Model* model = nullptr;
	// ビルボード
	bool isBillboard = false;
};

