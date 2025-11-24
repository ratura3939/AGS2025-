#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../Common/Geometry/Model.h"
#include "Cross.h"

//ローカル定数1
namespace {
	const std::string OBJ_NAME = "Cross";
	const VECTOR INIT_SCL = { 0.3f,0.3f,0.3f };
}

Cross::Cross(const VECTOR& _pos)
{
	pos_ = _pos;
}

Cross::~Cross(void)
{
}

void Cross::HitCollider(std::weak_ptr<Collider> _col)
{
}

void Cross::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::CROSS_MDL).handleId_;
	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE}, std::move(std::make_unique<Model>(pos_, quaRot_, quaRotLocal_, scl_, modelId_)));
}
