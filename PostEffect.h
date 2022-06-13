#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    // コンストラクタ
    PostEffect();

    // 初期化
    void Initialize();

    // 描画コマンドの発行
    void Draw(ID3D12GraphicsCommandList* cmdList);

    // テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;

    // SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

