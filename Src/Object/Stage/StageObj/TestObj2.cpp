#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../Common/Geometry/Sphere.h"
#include "TestObj2.h"

//ローカル定数
namespace {
	const float SPHERE_RADIUS = 20.0f;	//コライダー半径
	const std::string OBJ_NAME = "Axe";
}

TestObj2::TestObj2(void)
{
	speciesName_ = OBJ_NAME;
}

TestObj2::~TestObj2(void)
{
}

void TestObj2::HitCollider(std::weak_ptr<Collider> _col)
{
}

void TestObj2::SetParam(void)
{
	isAffectAbilyty_ = true;
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TEST_MDL_2).handleId_;
	pos_ = { -200.0f,100.0f,500.0f };
	scl_ = { 15.0f,15.0f ,15.0f };

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Sphere>(pos_, SPHERE_RADIUS)));

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(NOMAL_COLOR);

	//デバッグ
	isDrawScreenPosCircle_ = true;
	screenPosColor_ = 0xff00ff;

	isActiveGravity_ = false;
}
