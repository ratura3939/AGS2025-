#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Common/Geometry/Cube.h"
#include"../../StageManager.h"
#include "Block.h"

//ローカル定数
namespace {
	const std::string OBJ_NAME = "Block";
	const VECTOR INIT_SCL = { 1.0f,1.0f,1.0f };
	const VECTOR COLLIDER_SIZE = VScale({ 200.0f,260.0f,200.0f },StageManager::INIT_MASTER_SCALE);
}

Block::Block(const VECTOR& _pos, const VECTOR& _size)
{
	pos_ = _pos;
	scl_ = _size;
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
	modelId_ = resM.LoadModelDuplicate(ResourceManager::SRC::BLOCK_MDL);
	//scl_ = INIT_SCL;

	//コライダー設定
	using TAG = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<TAG>{TAG::STAGE}, std::move(std::make_unique<Cube>(pos_, quaRot_, Utility::VMul(COLLIDER_SIZE, scl_))), std::set<TAG>{TAG::STAGE, TAG::ATTACK, TAG::PREATTACK, TAG::SWITCH});
}
