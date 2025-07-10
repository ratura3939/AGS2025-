//スキンメッシュ*ブラー対応
// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_8FRAME

#define L_W_MAT g_localWorldMatrix.lwMatrix

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"


cbuffer BoneMat:register(b7) {
	float4x4 g_currentLocalWorldMatrix[256]; // 現在のボーン行列
	float4x4 g_prevLocalWorldMatrix[256];    // 前フレームのボーン行列
}

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;
	 // --- 現在のフレームのスキンメッシュ変換行列を構築 ---
	//// BONE4
	float4x4 current_CombinedBoneMatrix = (float4x4)0;
	int4 lBoneIdx = VSInput.blendIndices0;
	float4 lWeight = VSInput.blendWeight0;
	current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.x] * lWeight.xxxx;
	current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.y] * lWeight.yyyy;
	current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.z] * lWeight.zzzz;
	current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.w] * lWeight.wwww;

	// --- 前フレームのスキンメッシュ変換行列を構築 ---
	float4x4 prev_CombinedBoneMatrix = (float4x4)0;
	prev_CombinedBoneMatrix += g_prevLocalWorldMatrix[lBoneIdx.x] * lWeight.xxxx;
	prev_CombinedBoneMatrix += g_prevLocalWorldMatrix[lBoneIdx.y] * lWeight.yyyy;
	prev_CombinedBoneMatrix += g_prevLocalWorldMatrix[lBoneIdx.z] * lWeight.zzzz;
	prev_CombinedBoneMatrix += g_prevLocalWorldMatrix[lBoneIdx.w] * lWeight.wwww;


	//// BONE8
	lBoneIdx = VSInput.blendIndices1;
	lWeight = VSInput.blendWeight1;
	current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.x] * lWeight.xxxx;
	current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.y] * lWeight.yyyy;
	current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.z] * lWeight.zzzz;
	current_CombinedBoneMatrix += g_currentLocalWorldMatrix[lBoneIdx.w] * lWeight.wwww;

	// 現在のワールド座標
	float4 lCurrentWorldPosition = mul(lLocalPosition, current_CombinedBoneMatrix);
	ret.worldPos = lCurrentWorldPosition.xyz;
	ret.svPos = mul(lCurrentWorldPosition, g_currentViewProjectionMatrix); // g_currentViewProjectionMatrix は BaseMatrices から

	// --- 前フレームのスキンメッシュ変換行列を構築 ---
	prev_CombinedBoneMatrix += g_prevLocalWorldMatrix[lBoneIdx.x] * lWeight.xxxx;
	prev_CombinedBoneMatrix += g_prevLocalWorldMatrix[lBoneIdx.y] * lWeight.yyyy;
	prev_CombinedBoneMatrix += g_prevLocalWorldMatrix[lBoneIdx.z] * lWeight.zzzz;
	prev_CombinedBoneMatrix += g_prevLocalWorldMatrix[lBoneIdx.w] * lWeight.wwww;

	// 前フレームのワールド座標
	float4 lPrevWorldPosition = mul(lLocalPosition, prev_CombinedBoneMatrix);
	ret.prevSvPos = mul(lPrevWorldPosition, g_prevViewProjectionMatrix); // g_prevViewProjectionMatrix は BaseMatrices から


	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;

	// float3 → float4
	lLocalPosition.xyz = VSInput.pos;
	lLocalPosition.w = 1.0f;

	//lLocalPosition.xyz += (VSInput.norm * 2.0f);

	// ローカル座標をワールド座標に変換
	lWorldPosition.w = 1.0f;
	//lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
	lWorldPosition.x = dot(lLocalPosition, lL_W_Mat[0]);
	lWorldPosition.y = dot(lLocalPosition, lL_W_Mat[1]);
	lWorldPosition.z = dot(lLocalPosition, lL_W_Mat[2]);
	ret.worldPos = lWorldPosition.xyz;

	// ワールド座標をビュー座標に変換
	lViewPosition.w = 1.0f;
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos.xyz = lViewPosition.xyz;

	// ビュー座標を射影座標に変換
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);


	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
	// UV座標
	ret.uv.x = VSInput.uv0.x;
	ret.uv.y = VSInput.uv0.y;

	// 法線
	// 法線をローカル空間からワールド空間へ変換
	//ret.normal = normalize(
	//mul(VSInput.norm, (float3x3)g_base.localWorldMatrix));
	ret.normal.x = dot(VSInput.norm, lL_W_Mat[0]);
	ret.normal.y = dot(VSInput.norm, lL_W_Mat[1]);
	ret.normal.z = dot(VSInput.norm, lL_W_Mat[2]);

	// ディフューズカラー
	ret.diffuse = VSInput.diffuse;
	// ライト方向(ローカル)
	ret.lightDir = float3(0.0f, 0.0f, 0.0f);
	// ライトから見た座標
	ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了




	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )
	// 出力パラメータを返す

	return ret;
}