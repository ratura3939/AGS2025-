#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Common/Geometry/Model.h"
#include"../../../Common/Geometry/Cube.h"
#include "Stairs.h"

//ローカル定数
namespace {
	const std::string OBJ_NAME = "Stairs";
	const VECTOR INIT_SCL = { 1.5f,1.5f,1.5f };
	const float COLLIDER_DEG = 55.0f;
	const VECTOR COLLIDER_SIZE = { 180.0f,250.0f,170.0f };
	const VECTOR COLLIDER_POS_OFFSET = { 0.0f,-40.0f,50.0f };
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
}

void Stairs::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::STAIRS_LOW_MDL).handleId_;
	scl_ = INIT_SCL;

	stairsColliderQua_ = Quaternion::Euler(Utility::Deg2RadF(COLLIDER_DEG), 0.0f, 0.0f);
	stairsColliderPos_ = VAdd(pos_, Quaternion::PosAxis(rot_, COLLIDER_POS_OFFSET));

	//コライダー設定
	using TAG = Collider::COL_TAG;

	collider_ = std::make_shared<Collider>(*this, std::set<TAG>{TAG::STAGE,TAG::STAIRS}, std::move(std::make_unique<Cube>(stairsColliderPos_, stairsColliderQua_, Utility::VMul(COLLIDER_SIZE,scl_))));
}
