#include"../StageManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"
#include "GimmickObjBase.h"

GimmickObjBase::GimmickObjBase(void)
	: moveDir_({ 0.0f,0.0f,0.0f })
	, isAffectAbilyty_(true)
	, isTargeting_(false)
	, screenPos_({ 0.0f,0.0f,0.0f })
	, isAffectingNow_(false)
	, update_(&GimmickObjBase::UpdateNomal)
{
}

GimmickObjBase::~GimmickObjBase(void)
{

}

void GimmickObjBase::DoInit(void)
{
	//各オブジェクトの設定
	SetParam();
	pos_ = VScale(pos_, StageManager::INIT_MASTER_SCALE);
	scl_ = VScale(scl_, StageManager::INIT_MASTER_SCALE);

	MV1SetPosition(modelId_, pos_);
	MV1SetScale(modelId_, scl_);

	render_ = std::make_unique<ModelRenderer>(modelId_, *material_);

	gravity_.y = GRAVITY_POW;
}

void GimmickObjBase::DoUpdate(void)
{
	(this->*update_)();
}

void GimmickObjBase::DecMoveSpeed(void)
{
	moveSpeed_+= MOVE_SPEED_DEC;
	if (moveSpeed_ < 0.0f) {
		moveSpeed_ = 0.0f;
	}
}

void GimmickObjBase::Draw(void)
{
	render_->Draw();

	////スクリーン座標位置(デバッグ用)
	if (isDrawScreenPosCircle_) {
		screenPos_ = ConvWorldPosToScreenPos(pos_);
		//DrawCircle(screenPos_.x, screenPos_.y, 10, screenPosColor_);
	}

	//DrawDebug();
}

void GimmickObjBase::DrawDebug(void) 
{
	collider_->DrawDebugCollider();
}


void GimmickObjBase::Release(void)
{
}

void GimmickObjBase::SetObjectRenderColor(const FLOAT4& _color)
{
	material_->SetConstBufPS(0, _color);
}

const VECTOR& GimmickObjBase::GetScreenPos(void) const
{
	return screenPos_;
}

const int GimmickObjBase::GetModelId(void) const
{
	return modelId_;
}

const bool GimmickObjBase::IsAffectAbility(void) const
{
	return isAffectAbilyty_;
}

void GimmickObjBase::SetIsAffecting(const bool _flag)
{
	isAffectingNow_ = _flag;
	if (_flag) {
		isActiveGravity_ = false;
	}
	else isActiveGravity_ = true;
}

const bool GimmickObjBase::IsAffecting(void) const
{
	return isAffectingNow_;
}

void GimmickObjBase::AffectedLockTime(void)
{
	update_ = &GimmickObjBase::UpdateAffectLock;
}

void GimmickObjBase::AffectedMagnet(void)
{
	update_ = &GimmickObjBase::UpdateAffectMagnet;
}

void GimmickObjBase::FinishAffect(void)
{
	update_ = &GimmickObjBase::UpdateNomal;
}

void GimmickObjBase::UpdateNomal(void)
{
	power_ = moveSpeed_;
	VECTOR moveVec = { 0.0f,0.0f,0.0f };
	//移動量減衰
	DecMoveSpeed();
	//移動量の付与
	moveVec = VScale(moveDir_, moveSpeed_);
	pos_ = VAdd(pos_, moveVec);
}

void GimmickObjBase::UpdateAffectLock(void)
{
	//ベクトル計算
}

void GimmickObjBase::UpdateAffectMagnet(void)
{
}
