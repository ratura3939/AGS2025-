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
	stateCnt_ = 0;
	stateLimit_ = 0;
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

	//状態管理
	//通常じゃないとき
	if (character_->GetState() != PlayerChara::STATE::NOMAL) {
		//カウンタが状態の上限時間を上回っていたら
		if (stateCnt_ > stateLimit_) {
			//通常に戻す
			character_->SetState(PlayerChara::STATE::NOMAL);
		}
		stateCnt_++;
	}

	//プレイヤーからの入力
	UserInput(_atk);
	//キャラクター更新
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

void PlayerManager::UserInput(AttackManager& _atk)
{
	//プレイヤーからの入力総まとめ
	InputManager& ins = InputManager::GetInstance();
	//攻撃の生成
	if (ins.IsTrigerred("attack")) {
		_atk.Attack(ATTACK_NOMAL, 1.0f, VAdd(character_->GetPos(), character_->GetQua().PosAxis({ 0.0f, 75.0f, 100.0f })), character_->GetQua(), AttackManager::ATTACK_MASTER::PLAYER, 70.0f);
		character_->SetState(PlayerChara::STATE::ATTACK);
		//時間の設定
		RedyStateCount(_atk.GetTotalTime(ATTACK_NOMAL));
	}

	//回避入力があったとき(ロックオン状態でしか作動しない)
	if (IsAvoidMove() && ins.IsTrigerred("jump") && character_->IsRock()) {
		//回避状態に
		character_->SetState(PlayerChara::STATE::AVOID);
		//時間の設定
		RedyStateCount(LIMIT_AVOID_STATE);
	}
}


void PlayerManager::RedyStateCount(const int _limit)
{
	//カウンターの初期化
	stateCnt_ = 0;
	//上限時間の設定
	stateLimit_ = _limit;
}

const bool PlayerManager::IsAvoidMove(void) const
{
	InputManager& ins = InputManager::GetInstance();
	return ins.IsPressed("right") || ins.IsPressed("left") || ins.IsPressed("down");
}

const bool PlayerManager::IsAlive(void) const
{
	return character_->IsAlive();
}




void PlayerManager::DrawDebug(void)
{
	character_->DrawDebug();
}
