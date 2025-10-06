#include"../../Manager/Decoration/SoundManager.h"
#include"../AbilityManager.h"
#include "LockTime.h"

namespace {
	const int LOCK_TIME_MAX = 100;
	const FLOAT4 ABILITY_COLOR = { 1.0f,1.0f,0.0f,1.0f };
}

LockTime::LockTime(AbilityManager& _mng) :AbilityBase(_mng)
{
	timer_ = 0;
	lockObject_ = false;
}

LockTime::~LockTime(void)
{
}

void LockTime::Update(std::weak_ptr<GimmickObjBase> _obj)
{
	if (!lockObject_) {
		//オブジェクトの更新を止める

		lockObject_ = true;
	}

	timer_++;
	if (timer_ > LOCK_TIME_MAX) {
		//能力の自動解除
		manager_.EndUsingAbility();
	}

	
}

void LockTime::Draw(void)
{
}

void LockTime::ResetAbility(void)
{
	lockObject_ = false;
	timer_ = 0;
}
