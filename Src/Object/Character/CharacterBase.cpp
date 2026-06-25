#include"../../Utility/Utility.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include"../Common/Collider.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(void)
	:ActorBase()
	,material_(nullptr)
	,renderer_(nullptr)
	,atkCollider_(nullptr)
	,atkPos_(Utility::VECTOR_ZERO)
	,animController_(nullptr)
	,headPos_(Utility::VECTOR_ZERO)
	,centerPos_(Utility::VECTOR_ZERO)
	,footPos_(Utility::VECTOR_ZERO)
	,uiPos_(Utility::VECTOR_ZERO)
	,characterRotY_(Quaternion())
	,goalQua_(Quaternion())
	,stepRotation_(0.0f)
	,hp_(-1.0f)

{
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::UpdateAnimOnly(void)
{
	animController_->Update();
}

void CharacterBase::Draw(void)
{
	//描画処理
	MV1DrawModel(modelId_);	//モデル描画
	DrawUI();				//UI描画
}

void CharacterBase::Release(void)
{
	
}

void CharacterBase::SetGoalRot(const float _rad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetAngle();

	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + _rad, Utility::AXIS_Y);
	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, characterRotY_);
	// しきい値より大きかったら
	if (angleDiff > THRESHOLD_ROT){
		//回転し始めに設定
		stepRotation_ = 0.0f;
	}
	goalQua_ = axis;
}

void CharacterBase::Rotation(void)
{
	//回転時間増加
	stepRotation_ +=PER_ROT;
	// 回転の球面補間
	characterRotY_ = Quaternion::Slerp(
		characterRotY_, goalQua_, stepRotation_);

	//初期化を行い新たな回転量を設定する
	quaRot_ = Quaternion();
	quaRot_ = quaRot_.Mult(characterRotY_);
}


const VECTOR& CharacterBase::GetHeight(void) const
{
	return headPos_;
}

const float CharacterBase::GetCollisionRadius(void) const
{
	return collider_->GetGeometry().GetRadius();
}

const Quaternion& CharacterBase::GetQua(void) const
{
	return characterRotY_;
}

void CharacterBase::SetQua(const Quaternion _qua)
{
	characterRotY_ = _qua;
}

std::weak_ptr<Collider> CharacterBase::GetAttackCollider(void)
{
	return atkCollider_;
}

const bool CharacterBase::IsAlive(void) const
{
	return hp_ > 0;
}

void CharacterBase::Death(void)
{
	hp_ = 0;
}

void CharacterBase::HitCollider(std::weak_ptr<Collider> _col)
{
	using TAG = Collider::COL_TAG;
	//ステージとの衝突
	if (_col.lock()->IsContainsAnyTag(std::set<TAG>{TAG::STAGE, TAG::OBJECT})) {

		Geometry& myGeo = collider_->GetGeometry();
		float radius = myGeo.GetRadius();
		//本来立つべき位置と現在位置の差分を取得
		const VECTOR hitPoint = myGeo.GetHitPoint();		//衝突位置
		VECTOR backPow = VSub(hitPoint, pos_);				//めり込み量
		VECTOR colNormal = myGeo.GetHitNormal();			//法線ベクトル

		//法線方向閾値
		const float threshold = 0.3f;
		colNormal = Utility::EpsilonCustomThreshold(colNormal, threshold);

		if(_col.lock()->IsContainsTag(TAG::STAIRS)){
			
			
			// 振動対策:減衰処理
			const float DAMPING_FACTOR = 0.4f;
			backPow.y = backPow.y * DAMPING_FACTOR;
		}

		const VECTOR diff = Utility::VAbs(VSub(prevPos_, hitPoint));

		if (colNormal.x > 0.0f)pos_.x = prevPos_.x;
		if (colNormal.y > 0.0f) {
			pos_.y += backPow.y;

			gravity_ = { 0.0f,0.0f,0.0f };
		}

		if (colNormal.z > 0.0f)pos_.z = prevPos_.z;
	}

	//派生クラス側の処理
	DoHitCollider(_col);
}