#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Utility/Utility.h"
#include "PlayerChara.h"

const bool PlayerChara::Init(void)
{
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_MDL).handleId_;
	if (modelId_ == -1)return false;

	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT),0.0f);

	rState_ = ROCK_STATE::NOMAL;

	//注視点の設定
	focusPoint_ = FOCUS_NOMAL;

	Init3DPram();

	return true;
}

void PlayerChara::Update(void)
{
	Move();
	Rotation();
	UpdateRotQuat();
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

void PlayerChara::DrawDebug(void)
{
	DrawFormatString(0, 40, 0xffffff, "pPos={%.1f,%.1f,%.1f}\npRot={%.1f,%.1f,%.1f}", pos_.x, pos_.y, pos_.z, rot_.x, rot_.y, rot_.z);
	DrawFormatString(0, 120, 0xffffff, "GoalRot={%.1f,%.1f,%.1f}", goalQua_.x, goalQua_.y, goalQua_.z);
	VECTOR rockPos = SceneManager::GetInstance().GetCamera().GetRockPos();
	float deg = Utility::AngleDeg(pos_, VSub(rockPos, pos_));
	if (pos_.x > rockPos.x)deg = 180.0f + (180.0f - deg);
	DrawFormatString(0, 140, 0xffffff, "RockDeg={%.1f}", deg);
}

void PlayerChara::Move(void)
{
	InputManager& ins = InputManager::GetInstance();
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera().GetRot();
	VECTOR dir = Utility::VECTOR_ZERO;

	float afterDeg = 0.0f;

	//キーボード入力
	if (ins.IsNew(KEY_INPUT_W)) {
		dir = cameraRot.GetForward();
		afterDeg = Utility::Deg2RadF(DEG_FORWARD);
	}
	if (ins.IsNew(KEY_INPUT_A)) {
		dir = cameraRot.GetLeft();
		afterDeg = Utility::Deg2RadF(DEG_LEFT);
	}
	if (ins.IsNew(KEY_INPUT_S)) {
		dir = cameraRot.GetBack();
		afterDeg = Utility::Deg2RadF(DEG_BACK);
	}
	if (ins.IsNew(KEY_INPUT_D)) {
		dir = cameraRot.GetRight();
		afterDeg = Utility::Deg2RadF(DEG_RIGHT);
	}

	//移動が行われていたら
	if (!Utility::EqualsVZero(dir)) {
		pos_ = VAdd(pos_, VScale(dir, MOVE_POW));
		//回転量の設定
		if (rState_ == ROCK_STATE::ROCKON) {
			//ロックオン特有の角度設定
			VECTOR rockPos = SceneManager::GetInstance().GetCamera().GetRockPos();
			VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();

			//AngleDegは下から上方向にかけて左右どちらも0~180で角度をとる。
			//キャラクターの角度調整は右回りなので上記の関数で得た角度も右回り基準の0~360に変換しなければならない。
			//そのため基準であるプレイヤーが対象の右に来た時だけ調整を行うような処理を行っている
			float deg = Utility::AngleDeg(pos_, VSub(rockPos, pos_));
			if (pos_.x > rockPos.x)deg = Utility::CIRCLE_HALF_DEG + (Utility::CIRCLE_HALF_DEG - deg);
			afterDeg = Utility::Deg2RadF(deg)- cameraRot.y;
		}
		SetGoalRot(afterDeg);
	}
	
}
