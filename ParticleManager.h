#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "Camera.h"

// パーティクルマネージャ
class ParticleManager
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
	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale; // スケール
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ビュープロジェクション行列
		XMMATRIX matBillboard;	// ビルボード行列
	};

	// パーティクル1粒
	class Particle
	{
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
	public:
		// 座標
		XMFLOAT3 position = {};
		// 速度
		XMFLOAT3 velocity = {};
		// 加速度
		XMFLOAT3 accel = {};
		// 色
		XMFLOAT3 color = {};
		// スケール
		float scale = 1.0f;
		// 回転
		float rotation = 0.0f;
		// 初期値
		XMFLOAT3 s_color = {};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		// 最終値
		XMFLOAT3 e_color = {};
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		// 現在フレーム
		int frame = 0;
		// 終了フレーム
		int num_frame = 0;
	};

private: // 定数
	static const int vertexCount = 65536;		// 頂点数

public: // 静的メンバ関数
	// インスタンス生成
	static ParticleManager* Create(ID3D12Device* device, Camera* camera);

public: // メンバ関数	
	// 初期化
	void Initialize();
	
	// 毎フレーム処理
	void Update();

	// 描画
	void Draw(ID3D12GraphicsCommandList * cmdList);

	// パーティクルの追加
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale );

	// デスクリプタヒープの初期化
	void InitializeDescriptorHeap();

	// グラフィックパイプライン生成
	void InitializeGraphicsPipeline();

	// テクスチャ読み込み
	void LoadTexture();

	// モデル作成
	void CreateModel();

private: // メンバ変数
	// デバイス
	ID3D12Device* device = nullptr;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize = 0u;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// パーティクル配列
	std::forward_list<Particle> particles;
	// カメラ
	Camera* camera = nullptr;
private:
	// コンストラクタ
	ParticleManager(ID3D12Device* device, Camera* camera);
};

