#include"../../../../Manager/Generic/SceneManager.h"
#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Renderer/ModelMaterial.h"
#include"../../../../Renderer/ModelRenderer.h"
#include"../../../Common/Geometry/Model.h"
#include"../../StageManager.h"
#include "Gate.h"

//ローカル定数1
namespace {
	const float UV_SCALING_NOISE = 10.0f;
	const std::string OBJ_NAME = "Gate";
	const VECTOR INIT_SCL = { 0.6f,0.6f,0.6f };
	const VECTOR FENCE_RELATIVE_POS = { 0.0f,-1800.0f,0.0f };
	const float FENCE_MOVE_MAX = 800.0f * StageManager::INIT_MASTER_SCALE;
}

Gate::Gate(const VECTOR& _pos, const Quaternion& _qua)
{
	pos_ = _pos;
	quaRot_ = _qua;
	fence_ = std::make_unique<Fence>(VAdd(pos_, FENCE_RELATIVE_POS), quaRot_);
}

Gate::~Gate(void)
{
}

void Gate::Draw(void)
{
	fence_->Draw();
	fence_->DrawDebug();

	//経過時間
	material_->SetConstBufPS(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });
	//描画
	render_->Draw();
}

void Gate::DrawDebug(void)
{
	collider_->DrawDebugCollider();
	fence_->DrawDebug();

	const VECTOR fPos = fence_->GetPos();
	DrawFormatString(0, 300, 0xff0000, "Gate Pos={%.1f, %.1f, %.1f}\nFence Pos={%.1f, %.1f, %.1f}", pos_.x,pos_.y,pos_.z, fPos.x, fPos.y, fPos.z);
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

void Gate::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::GATE_MDL).handleId_;

	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE, COL_TYPE::SWITCH}, std::move(std::make_unique<Model>(pos_, quaRot_, quaRotLocal_, scl_, modelId_)));

	//フェンス
	fence_->Init();
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
