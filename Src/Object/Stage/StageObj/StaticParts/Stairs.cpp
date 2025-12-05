#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Common/Geometry/Model.h"
#include"../../../Common/Geometry/Cube.h"
#include "Stairs.h"

//ローカル定数1
namespace {
	const std::string OBJ_NAME = "Stairs";
	const VECTOR INIT_SCL = { 1.5f,1.5f,1.5f };
	const float COLLIDER_DEG = 60.0f;
	const VECTOR COLLIDER_SIZE = { 280.0f,400.0f,250.0f };
}

Stairs::Stairs(const VECTOR& _pos)
{
	pos_ = _pos;
}

Stairs::~Stairs(void)
{
}

void Stairs::HitCollider(std::weak_ptr<Collider> _col)
{
	//恐らくキューブ内のSetHitPosの問題によるものだと考えられる
}

void Stairs::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::STAIRS_LOW_MDL).handleId_;
	scl_ = INIT_SCL;

	stairsColliderQua_ = Quaternion::Euler(Utility::Deg2RadF(COLLIDER_DEG), 0.0f, 0.0f);

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE}, std::move(std::make_unique<Cube>(pos_, stairsColliderQua_, COLLIDER_SIZE)));
}
