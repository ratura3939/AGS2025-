#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/GameSystem/AnimationController.h"
#include"../../../Utility/Utility.h"
#include "PlayerChara.h"

PlayerChara::PlayerChara(void)
{
	focusPoint_ = Utility::VECTOR_ZERO;
	rState_ = ROCK_STATE::MAX;
	state_ = STATE::NOMAL;
}

PlayerChara::~PlayerChara(void)
{
}

const bool PlayerChara::Init(void)
{
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_MDL).handleId_;
	if (modelId_ == -1)return false;

	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT),0.0f);

	rState_ = ROCK_STATE::NOMAL;

	//注視点の設定
	focusPoint_ = FOCUS_NOMAL;

	UpdateRotQuat();
	hp_ = PALYER_HP;

	animController_ = std::make_unique<AnimationController>(modelId_);
	animController_->Add("idle", 36, AnimationController::PLAY_TYPE::LOOP);
	animController_->Play("idle", 1.0f);

	return true;
}

void PlayerChara::Update(void)
{
	//移動は何ほかにアクション行動していないときのみ
	if (state_ == STATE::NOMAL) {
		Move();
	}
	Rotation();
	UpdateRotQuat();

	animController_->Update();
}

const VECTOR PlayerChara::GetFocusPoint(void) const
{
	return VAdd(pos_, focusPoint_);
}

void PlayerChara::ChangeRockState(const bool _state)
{
	if (_state)rState_ = ROCK_STATE::ROCKON;
	else rState_ = ROCK_STATE::NOMAL;
}

const PlayerChara::STATE PlayerChara::GetState(void) const
{
	return state_;
}

void PlayerChara::SetState(const STATE& _state)
{
	state_ = _state;
}

const bool PlayerChara::IsRock(void)
{
	return rState_==ROCK_STATE::ROCKON;
}

void PlayerChara::DrawDebug(void)
{
	DrawFormatString(0, 40, 0xffffff, "pPos={%.1f,%.1f,%.1f}\npRot={%.1f,%.1f,%.1f}", pos_.x, pos_.y, pos_.z, rot_.x, rot_.y, rot_.z);
	DrawFormatString(0, 120, 0xffffff, "GoalRot={%.1f,%.1f,%.1f}", goalQua_.x, goalQua_.y, goalQua_.z);
	VECTOR rockPos = SceneManager::GetInstance().GetCamera().GetRockPos();
	float deg = Utility::AngleDeg(pos_, VSub(rockPos, pos_));
	if (pos_.x > rockPos.x)deg = 180.0f + (180.0f - deg);
	DrawFormatString(0, 140, 0xffffff, "RockDeg={%.1f}", deg);

	switch (state_)
	{
	case PlayerChara::STATE::NOMAL:
		DrawString(0, 160,  "NOMAL", 0xffffff);
		break;
	case PlayerChara::STATE::GUARD:
		DrawString(0, 160, "GUARD", 0xffffff);
		break;
	case PlayerChara::STATE::AVOID:
		DrawString(0, 160, "AVOID", 0xffffff);
		break;
	case PlayerChara::STATE::ATTACK:
		DrawString(0, 160, "ATTACK", 0xffffff);
		break;
	default:
		break;
	}

	DrawCupcel();
}

void PlayerChara::AnimInit(void)
{
}

void PlayerChara::Move(void)
{
	InputManager& ins = InputManager::GetInstance();
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera().GetRot();
	VECTOR dir = Utility::VECTOR_ZERO;

	float afterDeg = 0.0f;

	//入力
	if (ins.IsPressed("up")) {
		dir = cameraRot.GetForward();
		afterDeg = Utility::Deg2RadF(DEG_FORWARD);
	}
	if (ins.IsPressed("left")) {
		dir = cameraRot.GetLeft();
		afterDeg = Utility::Deg2RadF(DEG_LEFT);
	}
	if (ins.IsPressed("down")) {
		dir = cameraRot.GetBack();
		afterDeg = Utility::Deg2RadF(DEG_BACK);
	}
	if (ins.IsPressed("right")) {
		dir = cameraRot.GetRight();
		afterDeg = Utility::Deg2RadF(DEG_RIGHT);
	}

	//移動が行われていたら
	if (!Utility::EqualsVZero(dir)) {
		//移動処理
		pos_ = VAdd(pos_, VScale(dir, MOVE_POW));
		//上下の移動が起きない様に
		//ゆくゆくは重力とステージの当たり判定で処理する
		pos_.y = 0.0f;

		//ロックオンのとき
		if (rState_ == ROCK_STATE::ROCKON) {
			//ロックオン特有の角度設定
			VECTOR rockPos = SceneManager::GetInstance().GetCamera().GetRockPos();			//ロックオン対象位置	
			VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度

			//自分から対象へのベクトル
			auto diff = VSub(rockPos, pos_);
			//角度求める
			afterDeg = atan2(diff.x, diff.z) - cameraRot.y;
		}
		//目標角度設定
		SetGoalRot(afterDeg);
	}
	
}
