#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Utility/Utility.h"
#include"../../Common/Geometry/Sphere.h"
#include"../StageManager.h"
#include "TestObj.h"

//ローカル定数
namespace {
	const float SPHERE_RADIUS = 150.0f*StageManager::INIT_MASTER_SCALE;	//コライダー半径
	const std::string OBJ_NAME = "Moon";
	const float ATTACK_POW_DEC_RATE = 40.0f;	//攻撃力減衰率
	const VECTOR INIT_SCALE = { 0.6f,0.6f ,0.6f };	//初期スケール
	const VECTOR INIT_POS = { -3900.0f,1500.0f ,-5400.0f };	//初期スケール
}

void TestObj::DoHitCollider(const std::weak_ptr<Collider>& _col)
{
	if(_col.lock()->IsContainsTag(Collider::COL_TAG::ATTACK)){
		//攻撃に当たったときの処理
		//攻撃からのベクトルを付与
		moveDir_ = Utility::VNormalize(VSub(pos_, _col.lock()->GetGeometry().GetPos()));
		moveSpeed_ += _col.lock()->GetPower() / ATTACK_POW_DEC_RATE;
	}
}

void TestObj::SetModel(void)
{
	//月
	speciesName_ = OBJ_NAME;
	isAffectAbilyty_ = true;
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TEST_MDL).handleId_;
	pos_ = INIT_POS;
	scl_ = INIT_SCALE;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Sphere>(pos_, SPHERE_RADIUS)));
}
