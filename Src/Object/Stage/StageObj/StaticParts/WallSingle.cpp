#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../Common/Geometry/Model.h"
#include "WallSingle.h"

namespace {
	const VECTOR INIT_SCL = { 0.75f,0.75f,1.2f };
}

WallSingle::WallSingle(const VECTOR& _pos, const Quaternion& _qua)
{
	pos_ = _pos;
	quaRot_ = _qua;
}

WallSingle::~WallSingle(void)
{
}

void WallSingle::HitCollider(std::weak_ptr<Collider> _col)
{
}

void WallSingle::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.LoadModelDuplicate(ResourceManager::SRC::WALL_SINGLE_MDL);

	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE, COL_TYPE::LIBRA}, std::move(std::make_unique<Model>(pos_, quaRot_, quaRotLocal_, scl_, modelId_)));
}
