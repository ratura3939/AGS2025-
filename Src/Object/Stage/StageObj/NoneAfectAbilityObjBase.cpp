#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"
#include"../../Common/Geometry/Model.h"
#include "NoneAfectAbilityObjBase.h"

//ローカル定数1
namespace {
	const float UV_SCALING_NOISE = 10.0f;

	int BLACK = 0x000000;

	//シェーダーバッファ数
	const int NUM_CONST_BUF_VS = 0;
	const int NUM_CONST_BUF_PS = 3;
}

NoneAfectAbilityObjBase::NoneAfectAbilityObjBase(void)
{
}

NoneAfectAbilityObjBase::~NoneAfectAbilityObjBase(void)
{
}

void NoneAfectAbilityObjBase::Draw(void)
{
	//経過時間
	material_->SetConstBufPS(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });
	//描画
	render_->Draw();

	//DrawDebugCollider();
}

void NoneAfectAbilityObjBase::DrawDebugCollider(void)
{
	collider_->DrawDebugCollider();
}

void NoneAfectAbilityObjBase::SetParam(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	SetModel();

	CollisionManager::GetInstance().AddCollider(collider_);	//当たり判定登録

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", NUM_CONST_BUF_VS, "NoiseWavePS.cso", NUM_CONST_BUF_PS);
	//追加テクスチャ挿入
	material_->SetTextureBuf(ModelMaterial::SUB_TEX_1, resM.Load(ResourceManager::SRC::NOISE_STAGE).handleId_);
	//付与色
	material_->AddConstBufPS(NOMAL_COLOR);
	//経過時間
	material_->AddConstBufPS({ 0.0f,0.0f,0.0f,0.0f });
	//UV拡大率
	material_->AddConstBufPS({ UV_SCALING_NOISE,0.0f,0.0f,0.0f });

	isActiveGravity_ = false;

	//デバッグ
	isDrawScreenPosCircle_ = false;
	screenPosColor_ = BLACK;
}
