#include"../../Manager/Decoration/SoundManager.h"
#include"../../Utility/Utility.h"
#include"../AbilityManager.h"
#include "MagnetCatch.h"

namespace {
	const float ACC_DIREC = 0.1f;
	const float DIREC_MAX = 1.0f;
}

MagnetCatch::MagnetCatch(AbilityManager& _mng):AbilityBase(_mng)
{
	startPos_ = Utility::VECTOR_ZERO;
	goalPos_ = Utility::VECTOR_ZERO;
	isSetGoalPos_ = false;
	direcStep_ = 0.0f;
}

MagnetCatch::~MagnetCatch(void)
{
}

void MagnetCatch::Update(std::weak_ptr<GimmickObjBase> _obj)
{
}

void MagnetCatch::UpdateDirection(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos)
{
	//初手のゴール位置設定
	if (!isSetGoalPos_) {
		if (_obj.expired()) {
			goalPos_ = ConvScreenPosToWorldPos(AbilityManager::RETICLE_POS);
		}
		else {
			goalPos_ = _obj.lock()->GetPos();
		}
		isSetGoalPos_ = true;
	}

	//演出終了
	if (direcStep_ > DIREC_MAX) {
		//対象がないとき
		if (_obj.expired()) {
			manager_.RedyAbility();
		}
		else {
			//使用に遷移
			manager_.DoUse();
		}
	}
	
	//演出更新
	startPos_ = _playerPos;
	Utility::Lerp(startPos_, goalPos_, direcStep_);
	direcStep_ += ACC_DIREC;
}

void MagnetCatch::Draw(void)
{
	auto state = manager_.GetAbilityState();
	if (state == AbilityManager::STATE::DIRECTION || state == AbilityManager::STATE::USE) {

	}
}

void MagnetCatch::ResetAbility(void)
{
	startPos_ = Utility::VECTOR_ZERO;
	goalPos_ = Utility::VECTOR_ZERO;
	isSetGoalPos_ = false;
	direcStep_ = 0.0f;
}
