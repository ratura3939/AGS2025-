// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

SamplerState noiseSampler         : register(s11);      // ノイズテクスチャ
Texture2D    noiseTex             : register(t11);	    // ノイズテクスチャ

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;

	float g_time;
	float3 dmy_time;

	float g_noise_uv;
	float3 dmy_noise_uv;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	float4 color;
	float2 uv = PSInput.uv;

	//テクスチャーの色を取得
	color = diffuseMapTexture.Sample(diffuseMapSampler, uv);

	//ノイズテクスチャの色取得
	//UV値変動
	uv.xy *= g_noise_uv;
	uv.x += g_time * 0.1f;
	uv.y += sin(g_time) * 0.1f;
	
	float4 noisePow= noiseTex.Sample(diffuseMapSampler, uv);
	noisePow = (noisePow.x + noisePow.y + noisePow.z) / 3.0f;

	float4 waveCol = g_color * noisePow * 0.5f;

	//関数の戻り値がラスタライザに渡される
	return color + waveCol;
}