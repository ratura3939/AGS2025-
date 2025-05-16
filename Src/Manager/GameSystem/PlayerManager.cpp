#include"../../Object/Character/Player/PlayerChara.h"
#include"../Generic/InputManager.h"
#include"../Generic/SceneManager.h"
#include"../Generic/Camera.h"
#include"../../Scene/Game.h"
#include "PlayerManager.h"

PlayerManager::PlayerManager(Game& _gameScene):scene_(_gameScene)
{
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
	//キャラクター生成
	character_ = std::make_unique<PlayerChara>();
	character_->Init();
}

void PlayerManager::Update(void)
{
	//TODO
	// カメラのロックオンの処理の最適化

	//if (IsRockOnTrg()) {
	//	//ロックオン作動時
	//	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::ROCKON);
	//	character_->ChangeRockState(true);
	//	_parent->DecideRockEnemy();
	//}
	//else if (IsRockOffTrg()) {
	//	//ロックオン作終了時
	//	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
	//	character_->ChangeRockState(false);
	//}
	character_->Update();
}

void PlayerManager::Draw(void)
{
	character_->Draw();
}

void PlayerManager::Release(void)
{
	character_->Release();
}

const VECTOR PlayerManager::GetPos(void)
{
	return character_->GetPos();
}

const Quaternion PlayerManager::GetQua(void)
{
	return character_->GetQua();
}

const VECTOR PlayerManager::GetFocusPoint(void)
{
	return character_->GetFocusPoint();
}

const bool PlayerManager::IsRockOnTrg(void) const
{
	return InputManager::GetInstance().IsTrgDown(KEY_INPUT_R);
}

const bool PlayerManager::IsRockOffTrg(void) const
{
	return InputManager::GetInstance().IsTrgUp(KEY_INPUT_R);
}

void PlayerManager::RockOn(void)
{
	character_->ChangeRockState(true);
}

void PlayerManager::RockOff(void)
{
	character_->ChangeRockState(false);
}

void PlayerManager::DrawDebug(void)
{
	character_->DrawDebug();
}
