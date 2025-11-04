#include "GimmickObjBase.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"

GimmickObjBase::GimmickObjBase(void)
	:
	gravity_({ 0.0f,0.0f,0.0f })
	, moveVec_({ 0.0f,0.0f,0.0f })
	,isAffectAbilyty_(true)
	,isTargeting_(false)
	, prePos_({ 0.0f,0.0f,0.0f })
	, screenPos_({ 0.0f,0.0f,0.0f })
	, isActiveGravity_(true)
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

	MV1SetPosition(modelId_, pos_);
	MV1SetScale(modelId_, scl_);

	render_ = std::make_unique<ModelRenderer>(modelId_, *material_);

	gravity_.y = GRAVITY_POW;
}

void GimmickObjBase::DoUpdate(void)
{
	(this->*update_)();
}

void GimmickObjBase::Draw(void)
{
	render_->Draw();

	//スクリーン座標位置(デバッグ用)
	if (isDrawScreenPosCircle_) {
		screenPos_ = ConvWorldPosToScreenPos(pos_);
		DrawCircle(screenPos_.x, screenPos_.y, 10, screenPosColor_);
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
	if (isActiveGravity_) {
		prePos_ = pos_;
		//重力処理
		gravity_.y += GRAVITY_POW;
		moveVec_ = VAdd(moveVec_, gravity_);
		pos_ = VAdd(pos_, moveVec_);
	}
}

void GimmickObjBase::UpdateAffectLock(void)
{
	//ベクトル計算
}

void GimmickObjBase::UpdateAffectMagnet(void)
{
}
