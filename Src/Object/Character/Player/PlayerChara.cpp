#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Utility/Utility.h"
#include "PlayerChara.h"

void PlayerChara::SetPram(void)
{
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER).handleId_;

	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(180.0f),0.0f);

	rState_ = ROCK_STATE::ROCKON;

	//注視点の設定
	focusPoint_ = FOCUS_NOMAL;
}

void PlayerChara::Update(void)
{
	Move();
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
}

void PlayerChara::Move(void)
{
	InputManager& ins = InputManager::GetInstance();
	VECTOR dir = Utility::VECTOR_ZERO;

	//基本的にキャラクターの移動は前方方向のみであり左右に移動が入れられたときは回転を行い移動を始める形である

	//キーボード入力
	if (ins.IsNew(KEY_INPUT_W)) {
		dir = GetForward();
	}
	if (ins.IsNew(KEY_INPUT_A)) {
		dir = GetLeft();
	}
	if (ins.IsNew(KEY_INPUT_S)) {
		dir = GetBack();
	}
	if (ins.IsNew(KEY_INPUT_D)) {
		dir = GetRight();
	}

	//実際の移動処理
	pos_ = VAdd(pos_, VScale(dir, MOVE_POW));
}
