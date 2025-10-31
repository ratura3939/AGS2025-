#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../Common/Geometry/Sphere.h"
#include "TestObj.h"

//ローカル定数
namespace {
	const float SPHERE_RADIUS = 150.0f;	//コライダー半径
}

void TestObj::HitCollider(std::weak_ptr<Collider> _col)
{
	//ステージタグが存在するとき
	if(_col.lock()->IsContainsTag(Collider::COL_TAG::STAGE)){
		pos_ = prePos_;
		gravity_ = { 0.0f,0.0f,0.0f };
		//衝突した物体の法線方向に少し押し戻す
		moveVec_ = VScale(moveVec_, -0.3f);
	}
}

void TestObj::SetParam(void)
{
	//月
	isAffectAbilyty_ = true;
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TEST_MDL).handleId_;
	pos_ = { 200.0f,1500.0f,500.0f };
	scl_ = { 1.0f,1.0f ,1.0f };

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Sphere>(pos_, SPHERE_RADIUS)));
	CollisionManager::GetInstance().AddCollider(collider_);

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(NOMAL_COLOR);

	//デバッグ
	isDrawScreenPosCircle_ = true;
	screenPosColor_ = 0xff0000;
}

void TestObj::DrawDebug(void)
{
	collider_->DrawDebugCollider();
	VECTOR modelPos = MV1GetPosition(modelId_);
	DrawFormatString(10, 600, 0xffffff, "TestObj ModelPos X:%f Y:%f Z:%f", modelPos.x, modelPos.y, modelPos.z);
	DrawFormatString(10, 620, 0xffffff, "TestObj Pos       X:%f Y:%f Z:%f", pos_.x, pos_.y, pos_.z);
}
