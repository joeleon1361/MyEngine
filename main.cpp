#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sound.h"
#include "GameScreen.h"
#include "fbxsdk.h"
#include "PostEffect.h"
#include "FbxLoader.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	// 汎用機能
	WinApp *win = nullptr;
	DirectXCommon *dxCommon = nullptr;
	Input *input = nullptr;
	Audio *audio = nullptr;
	GameScene *gameScene = nullptr;
	PostEffect* postEffect = nullptr;
	FbxManager *fbxManager = FbxManager::Create();

	// ゲームウィンドウの作成
	win = new WinApp();
	win->CreateGameWindow();

	//DirectX初期化処理
	dxCommon = new DirectXCommon();
	dxCommon->Initialize( win );

#pragma region 汎用機能初期化
	// 入力の初期化
	input = new Input();
	if ( !input->Initialize( win->GetInstance(), win->GetHwnd() ) ) {
		assert( 0 );
		return 1;
	}
	// オーディオの初期化
	audio = new Audio();
	if ( !audio->Initialize() ) {
		assert( 0 );
		return 1;
	}
	// スプライト静的初期化
	if ( !Sprite::StaticInitialize( dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height ) ) {
		assert( 0 );
		return 1;
	}

	// ポストエフェクト用テクスチャの読み込み
	// Sprite::LoadTexture(100, L"Resources/white1x1.png");

	// ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize( dxCommon->GetDevice() );

	FbxLoader::GetInstance()->Initialize( dxCommon->GetDevice() );
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize( dxCommon, input, audio );

	// メインループ
	while ( true )
	{
		// メッセージ処理
		if ( win->ProcessMessage() ) { break; }

		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		gameScene->Update();

		// レンダーテクスチャへの描画
		// postEffect->PreDrawScene(dxCommon->GetCommandList());
		// gameScene->Draw();
		// postEffect->PostDrawScene(dxCommon->GetCommandList());

		// 描画開始
		dxCommon->PreDraw();

		// ポストエフェクトの描画
		// postEffect->Draw(dxCommon->GetCommandList());

		// ゲームシーンの描画
		gameScene->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}
	// 各種解放
	safe_delete( gameScene );
	safe_delete( audio );
	safe_delete( input );
	safe_delete( dxCommon );
	delete postEffect;
	FbxLoader::GetInstance()->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
	safe_delete( win );

	return 0;
}