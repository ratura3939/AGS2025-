#include"../../Object/Character/Player/PlayerChara.h"
#include"../Generic/InputManager.h"
#include"../Generic/SceneManager.h"
#include"../Generic/Camera.h"
#include"../GameSystem/AttackManager.h"
#include"../../Scene/Game.h"
#include "PlayerManager.h"

const std::string PlayerManager::ATTACK_NOMAL = "PlayerAttack";

PlayerManager::PlayerManager(Game& _gameScene):scene_(_gameScene)
{
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
	//キャラクター生成
	character_ = std::make_shared<PlayerChara>();
	character_->Init();
}

void PlayerManager::Update(AttackManager& _atk)
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

	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrigerred("action")) {
		_atk.Attack(ATTACK_NOMAL, 1.0f, VAdd(character_->GetPos(),character_->GetQua().PosAxis({ 0.0f, 75.0f, 100.0f }) ), character_->GetQua(), AttackManager::ATTACK_MASTER::PLAYER, 70.0f);
	}

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

std::weak_ptr<PlayerChara> PlayerManager::GetPlayer(void)
{
	return character_;
}

const VECTOR PlayerManager::GetFocusPoint(void)
{
	return character_->GetFocusPoint();
}

const bool PlayerManager::IsRockOnTrg(void) const
{
	return InputManager::GetInstance().IsPressed("rock");
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

const bool PlayerManager::IsAlive(void) const
{
	return character_->IsAlive();
}
