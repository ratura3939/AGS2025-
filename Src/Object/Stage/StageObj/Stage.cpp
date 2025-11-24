#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../Common/Geometry/Model.h"
#include "Stage.h"

//ローカル定数1
namespace {
	const std::string OBJ_NAME = "Stage";
	const VECTOR INIT_POS = { 0.0f,1400.0f,0.0f };
	const VECTOR INIT_SCL = { 0.75f,0.75f,0.75f };
}

Stage::Stage(void)
{
	isAffectAbilyty_ = false;
	speciesName_ = OBJ_NAME;
}
Stage::~Stage(void)
{
}

void Stage::HitCollider(std::weak_ptr<Collider> _col)
{
}

void Stage::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::STAGE_MDL).handleId_;
	pos_ = INIT_POS;
	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE}, std::move(std::make_unique<Model>(pos_, quaRot_, quaRotLocal_, scl_, modelId_)));	
}
