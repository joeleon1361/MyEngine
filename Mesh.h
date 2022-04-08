#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Material.h"
#include <vector>

// 形状データ
class Mesh
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
	// 頂点データ構造体（テクスチャあり）
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);	

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;	

public: // メンバ関数
	// 名前を取得
	const std::string& GetName() { return name; }
	
	// 名前をセット
	void SetName(const std::string& name);
	
	// 頂点データの追加
	void AddVertex(const VertexPosNormalUv& vertex);
	
	// 頂点インデックスの追加
	void AddIndex(unsigned short index);

	// 頂点データの数を取得
	inline size_t GetVertexCount() { return vertices.size(); }
	
	// マテリアルの取得
	Material* GetMaterial() { return material; }

	// マテリアルの割り当て
	void SetMaterial(Material* material);

	// バッファの生成
	void CreateBuffers();
	
	// 頂点バッファ取得
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView; }

	// インデックスバッファ取得
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView; }
	
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
	// 名前
	std::string name;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// マテリアル
	Material* material = nullptr;
};

