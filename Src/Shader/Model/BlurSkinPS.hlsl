#include"../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;		//各参考
	float g_blurAmount; // ブラーの強さ (0.0 ～ 1.0 など)
	float3 dummy_blur;
	float g_sampleCount; // サンプル数
	float3 dummy_smp;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	//float4 color;

	////テクスチャーの色を取得
	//color = diffuseMapTexture.Sample(diffuseMapSampler,PSInput.uv);
	//if (color.a < 0.01f) {
	//	discard;
	//}

	////どちらもワールド座標
	////法線
	//float3 nomal = PSInput.normal;
	////ライト方向
	////float3 lightDir = g_light.xyz;
	////lightDir.xyz = 1.0f;

	////float dotF = dot(nomal,lightDir);
	////dotF += 1.0f;
	////dotF /= 2.0f;

	////color.rgb *= (1 - dotF);

	////関数の戻り値がラスタライザに渡される
	//return color * g_color;

	//********************************************************************************************

	// 現在のクリップ空間座標をスクリーン空間座標に変換
	float2 currentScreenPos = PSInput.svPos.xy / PSInput.svPos.w;
	currentScreenPos = currentScreenPos * 0.5f + 0.5f; // [-1, 1] から [0, 1] へ変換

	// 前フレームのクリップ空間座標をスクリーン空間座標に変換
	float2 prevScreenPos = PSInput.prevSvPos.xy / PSInput.prevSvPos.w;
	prevScreenPos = prevScreenPos * 0.5f + 0.5f; // [-1, 1] から [0, 1] へ変換

	// 速度ベクトル（スクリーン空間）
	float2 velocity = currentScreenPos - prevScreenPos;

	// ブラーの強さを適用
	velocity *= g_blurAmount;

	float4 finalColor = float4(0, 0, 0, 0);

	// 複数のサンプルをサンプリングし、平均を取る
	for (int i = 0; i < g_sampleCount; ++i)
	{
		float t = (float)i / (g_sampleCount - 1.0f) - 0.5f; // -0.5 から 0.5 の範囲
		float2 sampleUV = PSInput.uv + velocity * t; // UV座標をずらしてサンプリング
		finalColor += diffuseMapTexture.Sample(diffuseMapSampler, sampleUV);
	}

	finalColor /= g_sampleCount;

	return finalColor * g_color;
}