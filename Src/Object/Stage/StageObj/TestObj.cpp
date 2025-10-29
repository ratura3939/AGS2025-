#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../Common/Geometry/Sphere.h"
#include "TestObj.h"

//ローカル定数
namespace {
	const float SPHERE_RADIUS = 50.0f;	//コライダー半径
}

void TestObj::HitCollider(const Collider::MASTER_TYPE& _hitType)
{
}

void TestObj::SetParam(void)
{
	//月
	isAffectAbilyty_ = true;
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TEST_MDL).handleId_;
	pos_ = { 200.0f,100.0f,500.0f };
	scl_ = { 1.0f,1.0f ,1.0f };

	//コライダー設定
	using COL_TYPE = Collider::MASTER_TYPE;
	collider_ = std::make_unique<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Sphere>(pos_, SPHERE_RADIUS)));

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(NOMAL_COLOR);

	//デバッグ
	isDrawScreenPosCircle_ = true;
	screenPosColor_ = 0xff0000;
}
