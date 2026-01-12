#include"../../../../Manager/Generic/SceneManager.h"
#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Utility/Utility.h"
#include"../../../Common/Geometry/Model.h"
#include"../../../Common/Geometry/Cube.h"
#include"../../StageManager.h"
#include "Fence.h"

//ローカル定数1
namespace {
	const float UV_SCALING_NOISE = 10.0f;
	const std::string OBJ_NAME = "Gate";
	const VECTOR INIT_POS = { 0.0f,0.0f,0.0f };
	const VECTOR INIT_SCL = { 0.4f,0.4f,0.4f };
	const float MOVE_SPEED = 6.0f;

	const VECTOR COLLIDER_SIZE = VScale({ 100.0f,400.0f,400.0f },StageManager::INIT_MASTER_SCALE);
	const VECTOR COLLIDER_DIFF_Y= VScale({ 0.0f,400.0f,0.0f },StageManager::INIT_MASTER_SCALE);
}

Fence::Fence(const VECTOR& _pos, const Quaternion& _rot)
	: colliderPos_(Utility::VECTOR_ZERO)
{
	pos_ = _pos;
}

Fence::~Fence(void)
{
}

void Fence::HitCollider(std::weak_ptr<Collider> _col)
{
}

void Fence::MoveFnece(const MOVE_DIR _dir)
{
	if(_dir == MOVE_DIR::UP){
		pos_.y += MOVE_SPEED;
		movedFenceDiff_ += MOVE_SPEED;
	}
	else if(_dir == MOVE_DIR::DOWN){
		pos_.y -= MOVE_SPEED;
		movedFenceDiff_ -= MOVE_SPEED;
	}
	colliderPos_ = VAdd(pos_, COLLIDER_DIFF_Y);
}

void Fence::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::FENCE_MDL).handleId_;
	scl_ = INIT_SCL;

	colliderPos_ = VAdd(VScale(pos_,StageManager::INIT_MASTER_SCALE), COLLIDER_DIFF_Y);

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE, COL_TYPE::SWITCH}, std::move(std::make_unique<Cube>(colliderPos_, quaRot_, COLLIDER_SIZE)), std::set<COL_TYPE>{COL_TYPE::STAGE});
}
