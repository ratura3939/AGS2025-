#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../Common/Geometry/Model.h"
#include "LibraPlate.h"

//ローカル定数
namespace {
	const VECTOR INIT_SCL = { 2.0f,2.0f,2.0f };
	const float MOVE_SPEED = 2.0f;
	const float PLATE_COLLIDER_HEIGHT = 50.0f;
	const float MOVE_LIMIT = 280.0f;
}

LibraPlate::LibraPlate(const VECTOR& _pos, const Quaternion& _qua, const float _offset)
{
	pos_ = _pos;
	pos_.y += _offset;
	quaRot_ = _qua;
	moveOffset_ = _offset;
}

LibraPlate::~LibraPlate(void)
{
}

void LibraPlate::HitCollider(std::weak_ptr<Collider> _col)
{
	using TAG = Collider::COL_TAG;

	//上に乗っているかの判定
	if (_col.lock()->IsContainsAnyTag(std::set<TAG>{TAG::OBJECT,TAG::PLAYER})) {
		//プレート上だったら
		if (pos_.y + PLATE_COLLIDER_HEIGHT < _col.lock()->GetGeometry().GetPos().y) {
			//積載量の更新
			currentLoadWeight_ += _col.lock()->GetWeight();
		}
	}
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
	currentLoadWeight_ = 0.0f;
	prevPos_ = pos_;

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

	//上限下限チェック
	ClampMoveOffset();
}

void LibraPlate::ClampMoveOffset(void)
{
	if (moveOffset_ > MOVE_LIMIT) {
		moveOffset_ = MOVE_LIMIT;
		pos_.y = prevPos_.y;
	}
	else if (moveOffset_ < -MOVE_LIMIT) {
		moveOffset_ = -MOVE_LIMIT;
		pos_.y = prevPos_.y;
	}
}
