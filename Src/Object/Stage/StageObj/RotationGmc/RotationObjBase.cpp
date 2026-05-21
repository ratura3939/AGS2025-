#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Renderer/ModelMaterial.h"
#include"../../../../Renderer/ModelRenderer.h"
#include"../../../../Utility/Utility.h"
#include "RotationObjBase.h"

namespace {
	const float ROTATION_SPEED = 0.02f;
	const float POWER_SCALING = 2.0f;
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
		moveSpeed_ += _col.lock()->GetPower() / POWER_SCALING;
		const std::string& atkName = _col.lock()->GetMasterName();
		CollisionManager::GetInstance().UseAttack(atkName);
	}
	
	//プレイヤーと当たっているとき
	if (_col.lock()->IsContainsTag(Collider::COL_TAG::PLAYER) || _col.lock()->IsContainsTag(Collider::COL_TAG::OBJECT) || _col.lock()->IsContainsTag(Collider::COL_TAG::ENEMY)) {
		//プレイヤー位置
		const VECTOR& colPos = _col.lock()->GetGeometry().GetPos();
		const float normalY = collider_->GetGeometry().GetHitNormal().y;

		//プレイヤーが上にいるとき
		if (normalY >= 1.0f && !isAffectingNow_) {
			//台に乗っているので
			//回転の影響を与える処理

			VECTOR addVec;	//影響ベクトル

			//台の中心からプレイヤーまでの相対座標
			VECTOR relativePos = VSub(colPos, pos_);

			//相対座標を回転させる
			//１フレーム後の回転量と現フレームの角度差をとる
			//１フレーム後のクォータニオンを作成し、QuaternionのAngleを使用して角度を取得
			float nextRotationPow = rotPow_;
			nextRotationPow += ROTATION_SPEED;
			if (moveSpeed_ > 0.0f) {
				nextRotationPow += (moveSpeed_ + MOVE_SPEED_DEC);
			}

			float oneFramePow = nextRotationPow - rotPow_;

			//回転
			Quaternion axis =
				Quaternion::AngleAxis(
					oneFramePow, Utility::AXIS_Y);

			//初期化を行い新たな回転量を設定する
			Quaternion quaRot = Quaternion();
			quaRot = quaRot.Mult(axis);

			VECTOR nextPos = quaRot.PosAxis(relativePos);

			//回転後-回転前で移動量が算出
			addVec = VSub(nextPos, relativePos);

			//影響を与える
			_col.lock()->AddExternalVecToMaster(addVec);
		}
	}
}

void RotationObjBase::SetParam(void)
{
	SetModel();
	isAffectAbilyty_ = true;

	CollisionManager::GetInstance().AddCollider(collider_);	//当たり判定登録

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(NOMAL_COLOR);

	isActiveGravity_ = false;
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
