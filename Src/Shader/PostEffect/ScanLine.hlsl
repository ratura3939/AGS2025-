#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;
	float  g_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float3 red = { 1.0f, 0.0f, 0.0f };
	float2 uv = PSInput.uv;

	// UV座標とテクスチャを参照して、最適な色を取得する
	float4 srcCol = tex.Sample(texSampler, uv);
	//そこの色の強さを赤の強さにする
    red.r *= (srcCol.r + srcCol.g + srcCol.b) / 3.0f;
	
	
	// 縦の大きさと時間で-1.0～1.0の値を作る
	float area = sin(uv.y * 2.0f - g_time * 0.5f);

	// 縦に特定範囲を作る
	// 1 or 0 …… 1 <= 2 = 1、2 <= 1 = 0 
	float isArea = step(0.996f, area * area);

    red.r -= abs(sin(uv.y * 60.0f + g_time * 1.0f)) * 0.05f;
    red.r -= abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;

    return float4(red, 1.0f);

}
