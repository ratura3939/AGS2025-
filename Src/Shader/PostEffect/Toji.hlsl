#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_time; // 経過時間（秒）
    float g_strength; // グリッチの強さ（0.0～1.0）
    float2 padding;
}

// 疑似乱数生成
float Rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 元のUV
    float2 uv = PSInput.uv;
    
    uv = float2(1 - uv.x, 1 - uv.y);

    // -----------------------------
    // 横ライン単位のグリッチずれ
    // -----------------------------
    // Y座標を粗く量子化（ライン単位）
    float lineIndex = floor(uv.y * 200.0f);

    // ラインごとのランダム値
    float glitchRand = Rand(float2(lineIndex, g_time));

    // 一定確率でずらす
    float glitchMask = step(0.85f, glitchRand);

    // 横方向のずれ量
    float offsetX = glitchMask * (glitchRand - 0.5f) * 0.05f * g_strength;

    uv.x += offsetX;

    // -----------------------------
    // RGBずらし
    // -----------------------------
    float2 uvR = uv + float2(0.003f * g_strength, 0.0f);
    float2 uvG = uv;
    float2 uvB = uv + float2(-0.003f * g_strength, 0.0f);

    float r = tex.Sample(texSampler, uvR).r;
    float g = tex.Sample(texSampler, uvG).g;
    float b = tex.Sample(texSampler, uvB).b;

    float4 col = float4(r, g, b, 1.0f);

    // -----------------------------
    // ノイズ付加
    // -----------------------------
    float noise = Rand(uv * g_time) - 0.5f;
    col.rgb += noise * 0.1f * g_strength;
    
    float3 useCol = (1.0 - col.rgb);

    return float4(useCol, 1.0f);
}
