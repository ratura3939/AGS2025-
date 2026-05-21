#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../Common/Geometry/Sphere.h"
#include "TestObj2.h"

//ローカル定数
namespace {
	const float SPHERE_RADIUS = 20.0f;	//コライダー半径
	const std::string OBJ_NAME = "Axe";
	const VECTOR INIT_SCALE = { 15.0f,15.0f ,15.0f };	//初期スケール
	const VECTOR INIT_POS = { -200.0f,100.0f,500.0f };	//初期スケール

	const int VS_CONST_BUF_NUM = 0;	//頂点シェーダー定数バッファの数
	const int PS_CONST_BUF_NUM = 1;	//定数バッファの数
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
	pos_ = INIT_POS;
	scl_ = INIT_SCALE;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Sphere>(pos_, SPHERE_RADIUS)));

	CollisionManager::GetInstance().AddCollider(collider_);	//当たり判定登録

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", VS_CONST_BUF_NUM, "StdModelPS.cso", PS_CONST_BUF_NUM);
	material_->AddConstBufPS(NOMAL_COLOR);

	isActiveGravity_ = false;
}
