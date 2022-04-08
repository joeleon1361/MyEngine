#pragma once

#include "fbxsdk.h"
#include "FbxModel.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class FbxLoader
{

private: // エイリアス
	// std::を省略
	using string = std::string;

public: // 定数
	// モデル格納ルートパス
	static const string baseDirectory;

	// テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

public: // 静的メンバ関数
	// シングルトンインスタンスの取得
	static FbxLoader *GetInstance();

	// FBXの行列をXMMatrixに変換
	static void ConvertMatrixFromFbx( DirectX::XMMATRIX *dst, const FbxAMatrix &src );

public: // メンバ関数
	void Initialize( ID3D12Device *device );

	void Finalize();
	
	FbxModel* LoadModelFromFile( const string &modelName );

	// 再帰的にノード構成を解析
	void ParseNodeRecursive( FbxModel *fbxmodel, FbxNode *fbxNode, Node* parent = nullptr );

	// メッシュ読み取り
	void ParseMesh( FbxModel *fbxmodel, FbxNode *fbxNode );

	// 頂点座標読み取り
	void ParseMeshVertices( FbxModel *fbxmodel, FbxMesh *fbxMesh );

	// 面情報読み取り
	void ParseMeshFaces( FbxModel *fbxmodel, FbxMesh *fbxMesh );

	// マテリアル読み取り
	void ParseMaterial( FbxModel *fbxmodel, FbxNode *fbxNode );

	// スキニング情報読み取り
	void ParseSkin( FbxModel *fbxmodel, FbxMesh *fbxMesh );

	// テクスチャ読み取り
	void LoadTexture( FbxModel *fbxmodel, const std::string &fullpath );

	// ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName( const std::string &path );

private: // メンバ変数
	// D3D12デバイス
	ID3D12Device *device = nullptr;

	// FBXマネージャー
	FbxManager *fbxManager = nullptr;

	// FBXインポータ
	FbxImporter *fbxImporter = nullptr;

	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
};