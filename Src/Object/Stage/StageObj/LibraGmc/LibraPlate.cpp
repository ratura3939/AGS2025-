#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../Common/Geometry/Model.h"
#include "LibraPlate.h"

//ローカル定数
namespace {
	const VECTOR INIT_SCL = { 1.0f,1.0f,1.0f };
	const float MOVE_SPEED = 2.0f;
}

LibraPlate::LibraPlate(const VECTOR& _pos, const Quaternion& _qua, const float _offset)
{
	pos_ = _pos;
	pos_.y += _offset;
	quaRot_ = _qua;
}

LibraPlate::~LibraPlate(void)
{
}

void LibraPlate::HitCollider(std::weak_ptr<Collider> _col)
{
	//上に乗っているかの判定

	//積載量の更新
}

void LibraPlate::SetState(const LIBRA_PLATE_STATE& _state)
{
	state_ = _state;
}

void LibraPlate::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.LoadModelDuplicate(ResourceManager::SRC::LIBRA_PLATE_MDL);

	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE, COL_TYPE::LIBRA}, std::move(std::make_unique<Model>(pos_, quaRot_, quaRotLocal_, scl_, modelId_)));
}

void LibraPlate::UpdateNomal(void)
{
	//動かない
	if (state_ == LIBRA_PLATE_STATE::STAY)return;

	//上昇
	if(state_ == LIBRA_PLATE_STATE::UP)
	{
		pos_.y += MOVE_SPEED;
		moveOffset_ += MOVE_SPEED;
	}
	//下降
	else
	{
		pos_.y -= MOVE_SPEED;
		moveOffset_ -= MOVE_SPEED;
	}
}
