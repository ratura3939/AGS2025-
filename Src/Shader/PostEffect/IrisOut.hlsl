#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color; //拡散光
    float g_time; //経過時間
    float3 dummy_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
   // 元の色を取得
    float4 col = tex.Sample(texSampler, PSInput.uv);
    
    const float2 centerUv = float2(0.5f, 0.5f);
    
    float diff = length(PSInput.uv - centerUv);
    
    float irisOutLine = 1.0f - diff;
    
    if (irisOutLine <= g_time)
    {
        col.rgb = g_color.rgb;
    }
    
    return col;
}