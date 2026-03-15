#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../AbilityManager.h"
#include "LockTime.h"

namespace {
	const int LOCK_TIME_MAX = 360;							//能力の効果時間
	const FLOAT4 ABILITY_COLOR = { 1.0f,1.0f,0.0f,1.0f };	//能力色
	const FLOAT4 ABILITY_COLOR_FINISH = { 0.0f,0.0f,0.0f,1.0f };	//能力色
	const float COLOR_WEAKNESS_RATE = 0.4f;					//点滅時どれ程弱くするか

	const int BEAT_NUM = 12;					//カウント時の拍数
	const int TEMPO = LOCK_TIME_MAX / BEAT_NUM;	//警告音の１拍の間隔(全体を)
	const int CHANGE_BEAT_INTERVAL_NUM = 4;		//何拍で間隔が変わるか
	const int CHANGE_BEAT_SPEED_RATE = 2;		//間隔を何倍にするか

	//効果音の音量調整
	const int COUNT_SOUND_VOLUME = 60;
	const int FINISH_SOUND_VOLUME = 40;
	const int FINISH_SOUND_PITICH_RATE = 600;

	//デバッグ用
	const FLOAT4 ABILITY_COLOR_DEBUG = { 1.0f,0.0f,1.0f,1.0f };
}

LockTime::LockTime(AbilityManager& _mng) :AbilityBase(_mng)
{
	timer_ = 0;
	alertBeatInterval_ = TEMPO;
	alertAfterCnt_ = 0;
	lockObject_ = false;
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.Add(SoundManager::TYPE::SE, "LockCount", ResourceManager::GetInstance().Load(ResourceManager::SRC::TIME_LOCK_SE).handleId_);
	sndM.AdjustVolume("LockCount", COUNT_SOUND_VOLUME);

	sndM.AdjustPitchRate(FINISH_SOUND_PITICH_RATE);

	sndM.Add(SoundManager::TYPE::SE, "BreakLock", ResourceManager::GetInstance().Load(ResourceManager::SRC::BREAK_TIME_LOCK_SE).handleId_);
	sndM.AdjustVolume("BreakLock", FINISH_SOUND_VOLUME);

	sndM.AdjustPitchRate();
	
}

LockTime::~LockTime(void)
{
	
}

void LockTime::UpdateUse(std::weak_ptr<GimmickObjBase> _obj)
{
	if (!lockObject_) {
		//オブジェクトの更新を止める
		_obj.lock()->AffectedLockTime();
		lockObject_ = true;
	}

	alertAfterCnt_++;
	timer_++;
	if (timer_ > LOCK_TIME_MAX) {
		//能力の自動解除
		manager_.ChangeState(AbilityManager::STATE::END);
		SoundManager::GetInstance().Play("BreakLock");
		return;
	}

	//警告表示
	if (timer_ % alertBeatInterval_ == 0) {
		//音をならす
		SoundManager::GetInstance().Play("LockCount");
		//黄色にする
		_obj.lock()->SetObjectRenderColor(ABILITY_COLOR);

		//警告したので初期化
		alertAfterCnt_ = 0;

		//指定の拍数を経過したら間隔を早くする
		if (timer_ % (TEMPO * CHANGE_BEAT_INTERVAL_NUM) == 0) {
			alertBeatInterval_ /= CHANGE_BEAT_SPEED_RATE;
		}
	}

	if (alertAfterCnt_ > alertBeatInterval_ / 2) {
		FLOAT4 weakColor = ABILITY_COLOR;
		weakColor.x *= COLOR_WEAKNESS_RATE;
		weakColor.y *= COLOR_WEAKNESS_RATE;
		weakColor.z *= COLOR_WEAKNESS_RATE;
		_obj.lock()->SetObjectRenderColor(weakColor);
	}
}

void LockTime::UpdateDirection(std::weak_ptr<GimmickObjBase> _obj)
{
	if (_obj.expired()) {
		manager_.ChangeState(AbilityManager::STATE::REDY);
	} 
	else{
		//オブジェクトに対しエフェクトをかける

		//使用に遷移
		manager_.ChangeState(AbilityManager::STATE::USE);
	}
}

void LockTime::Draw(void)
{
}

void LockTime::ResetAbility(void)
{
	timer_ = 0;
	alertBeatInterval_ = TEMPO;
	alertAfterCnt_ = 0;
	lockObject_ = false;
}
