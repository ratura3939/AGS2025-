#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"
#include"../../../Utility/Utility.h"
#include "AfectAbilityObjBase.h"

AfectAbilityObjBase::AfectAbilityObjBase(void)
{
}

AfectAbilityObjBase::~AfectAbilityObjBase(void)
{
}

void AfectAbilityObjBase::HitCollider(std::weak_ptr<Collider> _col)
{
	//ステージタグが存在するとき
	if (_col.lock()->IsContainsTag(Collider::COL_TAG::STAGE) && !isAffectingNow_) {
		//SetPrevPos();
		Geometry& myGeo = collider_->GetGeometry();

		const VECTOR hitPoint = myGeo.GetHitPoint();
		VECTOR backPow = VSub(hitPoint, pos_);

		VECTOR colNormal = myGeo.GetHitNormal();				//法線ベクトル

		const float threshold = 0.3f;
		VECTOR enablePrevPos = Utility::EpsilonCustomThreshold(colNormal, threshold);

		if (enablePrevPos.x > 0.0f)pos_.x = prevPos_.x;
		if (enablePrevPos.y > 0.0f)pos_.y = prevPos_.y;
		if (enablePrevPos.z > 0.0f)pos_.z = prevPos_.z;

		//衝突した物体の法線方向に少し押し戻す
		moveDir_ = collider_->GetGeometry().GetHitNormal();
		//moveSpeed_ = Utility::MagnitudeF(gravity_) * 0.5f;
		moveSpeed_ = backPow.y * 0.5f;

		gravity_ = { 0.0f,0.0f,0.0f };
	}

	DoHitCollider(_col);
}

void AfectAbilityObjBase::SetParam(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	SetModel();

	CollisionManager::GetInstance().AddCollider(collider_);	//当たり判定登録
	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(NOMAL_COLOR);
	//デバッグ
	isDrawScreenPosCircle_ = true;
	screenPosColor_ = 0xff0000;
}
