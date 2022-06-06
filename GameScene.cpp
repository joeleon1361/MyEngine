﻿#include "GameScene.h"
#include "Model.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"Camera.h"
#include <stdio.h>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(spriteBG);
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(objFighter);
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelFighter);

	safe_delete(fbxobject1);
	safe_delete(fbxmodel1);
	safe_delete(fbxobject3);
	safe_delete(fbxmodel3);
	safe_delete(testmodel);
	safe_delete(testobject);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);


	// カメラセット
	Object3d::SetCamera(camera);
	FbxObject3d::SetCamera(camera);


	// デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());

	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();


	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/background.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// パーティクルマネージャー
	particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	// 3Dオブジェクト生成
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objFighter = Object3d::Create();

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/Sprite/texture.png");

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("player2");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objFighter->SetModel(modelFighter);

	fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile("Fast Run");
	fbxmodel3 = FbxLoader::GetInstance()->LoadModelFromFile("Standing W_Briefcase Idle");
	testmodel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// FBX3Dオブジェクト生成とモデルとセット
	fbxobject1 = new FbxObject3d;
	fbxobject1->Initialize();
	fbxobject1->SetModel(fbxmodel1);

	fbxobject3 = new FbxObject3d;
	fbxobject3->Initialize();
	fbxobject3->SetModel(fbxmodel3);

	testobject = new FbxObject3d;
	testobject->Initialize();
	testobject->SetModel(testmodel);

	// 座標のセット
	testobject->SetRotation({ 0, 90, 0 });
}

void GameScene::Update()
{
	// オブジェクト移動
	if (input->PushKey(DIK_I) || input->PushKey(DIK_K) || input->PushKey(DIK_J) || input->PushKey(DIK_L))
	{
		// 現在の座標を取得
		XMFLOAT3 PlayerPosition1 = fbxobject1->GetPosition();
		XMFLOAT3 PlayerPosition3 = fbxobject3->GetPosition();

		// 移動後の座標を計算
		if (input->PushKey(DIK_I))
		{
			PlayerPosition1.z += 1.0f;
			PlayerPosition3.z += 1.0f;
		}
		else if (input->PushKey(DIK_K))
		{
			PlayerPosition1.z -= 1.0f;
			PlayerPosition3.z -= 1.0f;
		}
		if (input->PushKey(DIK_L))
		{
			PlayerPosition1.x += 1.0f;
			PlayerPosition3.x += 1.0f;
		}
		else if (input->PushKey(DIK_J))
		{
			PlayerPosition1.x -= 1.0f;
			PlayerPosition3.x -= 1.0f;
		}

		// 座標の変更を反映
		fbxobject1->SetPosition(PlayerPosition1);
		fbxobject3->SetPosition(PlayerPosition3);
	}

	// 現在の座標を取得
	XMFLOAT3 PlayerPosition1 = fbxobject1->GetPosition();

	// 座標の変更を反映
	fbxobject1->SetPosition(PlayerPosition1);

	MoveCamera();
	// パーティクル生成
	CreateParticles();

	camera->Update();
	particleMan->Update();

	objSkydome->Update();
	objGround->Update();
	objFighter->Update();

	fbxobject1->Update();
	fbxobject3->Update();
	testobject->Update();

	debugText.Print("AD: move camera LeftRight", 50, 50, 1.0f);
	debugText.Print("WS: move camera UpDown", 50, 70, 1.0f);
	debugText.Print("ARROW: move camera FrontBack", 50, 90, 1.0f);
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	// ここに背景スプライトの描画処理を追加できる

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	/* objSkydome->Draw();
	 objGround->Draw();*/
	 //objFighter->Draw();

	/*if (input->PushKey(DIK_I) || input->PushKey(DIK_K) || input->PushKey(DIK_J) || input->PushKey(DIK_L))
	{
		if (input->PushKey(DIK_I)) { fbxobject1->Draw(cmdList); }
		else if (input->PushKey(DIK_K)) { fbxobject1->Draw(cmdList); }
		if (input->PushKey(DIK_L)) { fbxobject1->Draw(cmdList); }
		else if (input->PushKey(DIK_J)) { fbxobject1->Draw(cmdList); }
	}
	else
	{
		fbxobject3->Draw(cmdList);
	}*/

	testobject->Draw(cmdList);

	// パーティクルの描画
	//particleMan->Draw(cmdList);

	// ここに3Dオブジェクトの描画処理を追加できる

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// ここに前景スプライトの描画処理を追加できる

	// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	// debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::MoveCamera()
{
	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W)) { camera->MoveVector({ 0.0f,+0.06f,0.0f }); }
		else if (input->PushKey(DIK_S)) { camera->MoveVector({ 0.0f,-0.06f,0.0f }); }
		if (input->PushKey(DIK_D)) { camera->MoveVector({ +0.06f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { camera->MoveVector({ -0.06f,0.0f,0.0f }); }
	}

	// カメラ移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		if (input->PushKey(DIK_UP)) { camera->MoveVector({ 0.0f,0.0f,+0.06f }); }
		else if (input->PushKey(DIK_DOWN)) { camera->MoveVector({ 0.0f,0.0f,-0.06f }); }
	}
}

void GameScene::CreateParticles()
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}
