#include "GimmickObjBase.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"

GimmickObjBase::GimmickObjBase(void)
	:
	gravity_(0.0f),
	isAffectAbilyty_(true),
	isTargeting_(false)
{
	screenPos_ = { 0.0f,0.0f,0.0f };
	isAffectingNow_ = false;
	isActiveGravity_ = true;
	update_ = &GimmickObjBase::UpdateNomal;
}

GimmickObjBase::~GimmickObjBase(void)
{
}

void GimmickObjBase::Init(void)
{
	//各オブジェクトの設定
	SetParam();

	MV1SetPosition(modelId_, pos_);
	MV1SetScale(modelId_, scl_);

	render_ = std::make_unique<ModelRenderer>(modelId_, *material_);
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

void GimmickObjBase::UpdateRotQuat(void)
{
	// 大きさ
	matScl_ = MGetScale(scl_);

	// 回転
	rot_ = quaRot_.ToEuler();
	matRot_ = quaRot_.ToMatrix();

	// 位置
	matPos_ = MGetTranslate(pos_);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// 行列をモデルに判定
	if (modelId_ != -1) {
		MV1SetMatrix(modelId_, mat);
	}
}

void GimmickObjBase::UpdateNomal(void)
{
	if (isActiveGravity_) {
		//重力処理
		gravity_ += GRAVITY_POW;
		pos_.y -= gravity_;
	}
}

void GimmickObjBase::UpdateAffectLock(void)
{
	//ベクトル計算
}

void GimmickObjBase::UpdateAffectMagnet(void)
{
}
