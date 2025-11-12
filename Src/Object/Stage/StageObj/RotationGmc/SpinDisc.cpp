#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Common/Collider.h"
#include"../../../Common//Geometry/Model.h"
#include "SpinDisc.h"

namespace {
	const std::string OBJ_NAME = "SpinDisc";	//オブジェクト名
	const VECTOR INIT_SCL = { 0.2f,0.2f,0.2f };
}

SpinDisc::SpinDisc(const VECTOR& _pos)
	: RotationObjBase(_pos)
{
}

SpinDisc::~SpinDisc(void)
{
}

void SpinDisc::SetModel(void)
{
	speciesName_ = OBJ_NAME;
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TURN_TABLE_MDL).handleId_;
	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Model>(pos_, quaRot_, modelId_)));
}
