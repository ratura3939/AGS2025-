#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Common/Geometry/Model.h"
#include "Stairs.h"

//ローカル定数1
namespace {
	const std::string OBJ_NAME = "Stairs";
	const VECTOR INIT_SCL = { 1.5f,1.5f,1.5f };
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

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE}, std::move(std::make_unique<Model>(pos_, quaRot_, quaRotLocal_, scl_, modelId_)));
}
