#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"
#include"../../Common/Geometry/Model.h"
#include "Stage.h"

//ローカル定数1
namespace {
	const float UV_SCALING_NOISE = 10.0f;
	const std::string OBJ_NAME = "Stage";
	const VECTOR INIT_POS = { 0.0f,1400.0f,0.0f };
	const VECTOR INIT_SCL = { 0.75f,0.75f,0.75f };
}

Stage::Stage(void)
{
	isAffectAbilyty_ = false;
	speciesName_ = OBJ_NAME;
}
Stage::~Stage(void)
{
}

void Stage::Draw(void)
{
	//経過時間
	material_->SetConstBufPS(1,{ SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });
	//描画
	render_->Draw();
}

void Stage::HitCollider(std::weak_ptr<Collider> _col)
{
}

void Stage::SetParam(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::STAGE_MDL).handleId_;
	pos_ = INIT_POS;
	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE}, std::move(std::make_unique<Model>(pos_, quaRot_, modelId_)));

	CollisionManager::GetInstance().AddCollider(collider_);	//当たり判定登録

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "NoiseWavePS.cso", 3);
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
	screenPosColor_ = 0x000000;
}
