#include"../../../../Manager/Generic/SceneManager.h"
#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Renderer/ModelMaterial.h"
#include"../../../../Renderer/ModelRenderer.h"
#include"../../../Common/Geometry/Model.h"
#include "Gate.h"

//ローカル定数1
namespace {
	const float UV_SCALING_NOISE = 10.0f;
	const std::string OBJ_NAME = "Gate";
	const VECTOR INIT_SCL = { 0.4f,0.4f,0.4f };
	const VECTOR FENCE_RELATIVE_POS = { 0.0f,-1200.0f,0.0f };
	const float FENCE_MOVE_MAX = 800.0f;
}

Gate::Gate(const VECTOR& _pos, const Quaternion& _qua)
{
	pos_ = _pos;
	fence_ = std::make_unique<Fence>(VAdd(pos_, FENCE_RELATIVE_POS), quaRot_);
	quaRot_ = _qua;
}

Gate::~Gate(void)
{
}

void Gate::Draw(void)
{
	fence_->Draw();

	//経過時間
	material_->SetConstBufPS(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });
	//描画
	render_->Draw();

	DrawSphere3D(pos_, 50.0f, 8, 0xffff0000, 0, false);
}

void Gate::HitCollider(std::weak_ptr<Collider> _col)
{
}

void Gate::SetObjectRenderColor(const FLOAT4& _color)
{
	material_->SetConstBufPS(0, _color);
	fence_->SetObjectRenderColor(_color);
}

void Gate::SetState(const GATE_STATE& _state)
{
	state_ = _state;
}

void Gate::SetParam(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::GATE_MDL).handleId_;

	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE, COL_TYPE::SWITCH}, std::move(std::make_unique<Model>(pos_, quaRot_, modelId_)));
	CollisionManager::GetInstance().AddCollider(collider_);

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

	//重力不要
	isActiveGravity_ = false;

	//フェンス
	fence_->Init();

	//デバッグ
	isDrawScreenPosCircle_ = false;
	screenPosColor_ = 0xffff000;
}

void Gate::UpdateNomal(void)
{
	if(state_ == GATE_STATE::OPEN){
		//開く処理
		fence_->MoveFnece(Fence::MOVE_DIR::UP);
	}
	else if(state_ == GATE_STATE::CLOSE){
		//閉じる処理
		fence_->MoveFnece(Fence::MOVE_DIR::DOWN);
	}

	const float fenveMovePow = fence_->GetMovedFenceDiff();
	if (fenveMovePow<0.0f || fenveMovePow>FENCE_MOVE_MAX) {
		SetState(GATE_STATE::NOMAL);
	}

	fence_->Update();
}
