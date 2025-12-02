#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color; 
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
   // 元の色を取得
    float2 uv = { 1.0f, 1.0f };
    float4 col = tex.Sample(texSampler, (uv - PSInput.uv));

    col = float4(1 - col.r, 1 - col.g, 1 - col.b, 1);

    return col;

}