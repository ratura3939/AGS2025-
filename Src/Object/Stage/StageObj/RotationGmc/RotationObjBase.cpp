#include"../../../../Renderer/ModelMaterial.h"
#include"../../../../Renderer/ModelRenderer.h"
#include"../../../../Utility/Utility.h"
#include "RotationObjBase.h"

namespace {
	const float ROTATION_SPEED = 0.02f;
}

RotationObjBase::RotationObjBase(const VECTOR& _pos)
	: rotPow_(0.0f)
{
	pos_ = _pos;
}

RotationObjBase::~RotationObjBase(void)
{
}

void RotationObjBase::HitCollider(std::weak_ptr<Collider> _col)
{
	if (_col.lock()->IsContainsTag(Collider::COL_TAG::ATTACK)) {
		//攻撃に当たったときの処理
		//攻撃からのベクトルを付与
		moveDir_ = Utility::VNormalize(VSub(pos_, _col.lock()->GetGeometry().GetPos()));
		moveSpeed_ += _col.lock()->GetPower();
	}
}

void RotationObjBase::SetParam(void)
{
	SetModel();
	isAffectAbilyty_ = true;

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(NOMAL_COLOR);

	isActiveGravity_ = false;

	//デバッグ
	isDrawScreenPosCircle_ = true;
	screenPosColor_ = 0xff0000;
}

void RotationObjBase::UpdateNomal(void)
{
	DecMoveSpeed();
	rotPow_ += ROTATION_SPEED;
	rotPow_ += moveSpeed_;
	//回転
	Quaternion axis =
		Quaternion::AngleAxis(
			rotPow_, Utility::AXIS_Y);

	//初期化を行い新たな回転量を設定する
	quaRot_ = Quaternion();
	quaRot_ = quaRot_.Mult(axis);
}
