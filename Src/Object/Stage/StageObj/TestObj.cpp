#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Utility/Utility.h"
#include"../../Common/Geometry/Sphere.h"
#include "TestObj.h"

//ローカル定数
namespace {
	const float SPHERE_RADIUS = 150.0f;	//コライダー半径
	const std::string OBJ_NAME = "Moon";
	const float ATTACK_POW_DEC_RATE = 30.0f;	//攻撃力減衰率
}

void TestObj::HitCollider(std::weak_ptr<Collider> _col)
{
	//ステージタグが存在するとき
	if(_col.lock()->IsContainsTag(Collider::COL_TAG::STAGE)){
		pos_ = prePos_;
		//衝突した物体の法線方向に少し押し戻す
		moveDir_ = collider_->GetGeometry().GetHitNormal();
		moveSpeed_ = Utility::MagnitudeF(gravity_) * 0.5f;

		gravity_ = { 0.0f,0.0f,0.0f };
	}

	if(_col.lock()->IsContainsTag(Collider::COL_TAG::ATTACK)){
		//攻撃に当たったときの処理
		//攻撃からのベクトルを付与
		moveDir_ = Utility::VNormalize(VSub(pos_, _col.lock()->GetGeometry().GetPos()));
		moveSpeed_ += _col.lock()->GetPower() / ATTACK_POW_DEC_RATE;
	}
}

void TestObj::SetParam(void)
{
	//月
	speciesName_ = OBJ_NAME;
	isAffectAbilyty_ = true;
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TEST_MDL).handleId_;
	pos_ = { 200.0f,1500.0f,500.0f };
	scl_ = { 1.0f,1.0f ,1.0f };

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Sphere>(pos_, SPHERE_RADIUS)));

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(NOMAL_COLOR);

	//デバッグ
	isDrawScreenPosCircle_ = true;
	screenPosColor_ = 0xff0000;
}

void TestObj::DrawDebug(void)
{
	//collider_->DrawDebugCollider();
	//VECTOR modelPos = MV1GetPosition(modelId_);
	//DrawFormatString(10, 600, 0xffffff, "TestObj ModelPos X:%f Y:%f Z:%f", modelPos.x, modelPos.y, modelPos.z);
	DrawFormatString(10, 620, 0xffffff, "TestObj Pos       X:%f Y:%f Z:%f", pos_.x, pos_.y, pos_.z);
}
