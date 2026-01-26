// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D fallTex : register(t11);          //ガイドラインテクスチャ
SamplerState fallSampler : register(s11);   //ガイドラインテクスチャ

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_time;       //経過時間
    float3 dmy_time;    //ダミー
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //モデルを上下反転させてるためY軸を反転
    float2 uv = PSInput.uv;
    uv.y = 1.0f - uv.y - g_time * 0.01f;
    
    float4 texCol = fallTex.Sample(diffuseMapSampler, uv);
    
    if (texCol.a < 0.1f)
    {
        discard;
    }
   
    
    return texCol;
}