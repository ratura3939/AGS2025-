#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Common/Geometry/Cube.h"
#include "Block.h"

//ローカル定数
namespace {
	const std::string OBJ_NAME = "Block";
	const VECTOR INIT_SCL = { 1.0f,1.0f,1.0f };
	const VECTOR COLLIDER_SIZE = { 200.0f,260.0f,200.0f };
}

Block::Block(const VECTOR& _pos)
{
	pos_ = _pos;
}

Block::~Block(void)
{
}

void Block::HitCollider(std::weak_ptr<Collider> _col)
{
}

void Block::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::BLOCK_MDL).handleId_;
	scl_ = INIT_SCL;

	//コライダー設定
	using TAG = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<TAG>{TAG::STAGE}, std::move(std::make_unique<Cube>(pos_, quaRot_, Utility::VMul(COLLIDER_SIZE, scl_))), std::set<TAG>{TAG::STAGE, TAG::ATTACK, TAG::PREATTACK, TAG::SWITCH});
}
