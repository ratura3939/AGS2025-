#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color; //拡散光
    float g_power; //フラシュの強さ
    float3 dummy_power;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
   // 元の色を取得
    float4 col = tex.Sample(texSampler, PSInput.uv);

   // 光のリングを加算
    float3 addWhite = (g_color.r * g_power, g_color.g * g_power, g_color.b * g_power);

    col.rgb += addWhite;
    
    return col;
}