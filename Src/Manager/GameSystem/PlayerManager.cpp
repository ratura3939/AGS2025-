#include"../../Object/Character/Player/PlayerChara.h"
#include"../Generic/InputManager.h"
#include"../Generic/SceneManager.h"
#include"../Generic/Camera.h"
#include"../Decoration/SoundManager.h"
#include"../GameSystem/AttackManager.h"
#include"../../PlayerSystem/LockOnManager.h"
#include"../../PlayerSystem/AbilityManager.h"
#include"../../Scene/Main/Game.h"
#include"../../Scene/Sub/SelectAbility.h"
#include"../../Utility/Utility.h"
#include "PlayerManager.h"

const std::string PlayerManager::ATTACK_NOMAL = "PlayerAttack";

PlayerManager::PlayerManager(Game& _gameScene, EnemyManager& _enemy, AttackManager& _atk, StageManager& _stage)
	:scene_(_gameScene)
	,atkMng_(_atk)
{
	character_ = std::make_shared<PlayerChara>(_atk);
	character_->Init();
	lockOn_ = std::make_unique<LockOnManager>(_gameScene, *this, _enemy);
	ability_ = std::make_unique<AbilityManager>(_stage, *character_);
	stateCnt_ = 0;
	stateLimit_ = 0;
	abilityBtnCnt_ = 0;
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
	//攻撃とコライダーの紐づけ
	atkMng_.AddAttackCollider(character_->GetSpeciesName(), character_->GetAttackCollider(), false, ATTACK_TIME);
}

void PlayerManager::Update(AttackManager& _atk)
{
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

	//スロー状態管理
	if (character_->IsSlow()) {
		scene_.StartSlow();
		character_->SetIsSlow(false);
	}

	//プレイヤーからの入力
	UserInput(_atk);
	//キャラクター更新
	character_->Update();
	//ロックオン更新
	lockOn_->Update();
	//能力更新
	ability_->Update();
}

void PlayerManager::Draw(void)
{
	character_->Draw();
	ability_->Draw();
}

void PlayerManager::Release(void)
{
	character_->Release();
}

const VECTOR& PlayerManager::GetPos(void)const
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

const VECTOR PlayerManager::GetFollowPos4UseMagnet(void)
{
	return ability_->GetFollowPos4UseMagnet(character_->GetPos());
}


void PlayerManager::RedyLockOn(void)
{
	character_->ChangeLockState(true);
}

void PlayerManager::RedyLockOff(void)
{
	character_->ChangeLockState(false);
}

void PlayerManager::UserInput(AttackManager& _atk)
{
	//プレイヤーからの入力総まとめ
	InputManager& ins = InputManager::GetInstance();

#pragma region 移動
	//移動
	//入力
	//入力があったら対応した移動方向をセット
	if (ins.IsPressed("up")) {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::FORWARD);
	}
	else if (ins.IsPressed("left")) {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::LEFT);
	}
	else if (ins.IsPressed("down")) {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::BACK);
	}
	else if (ins.IsPressed("right")) {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::RIGHT);
	}
	//移動していないとき
	else {
		character_->InputMoveDir(PlayerChara::MOVE_DIR::NONE);
	}
	//ダッシュ
	character_->InputDash(ins.IsPressed("dash"));
#pragma endregion

#pragma region 戦闘関連
	//攻撃中は入力を受け付けない
	if (character_->GetState() == PlayerChara::STATE::ATTACK)return;

	auto abilityState = ability_->GetAbilityState();

	//能力が使用されていない、又は許可されているとき入力を受け付ける
	if (abilityState == AbilityManager::STATE::END || ability_->IsPlayerAnyInput()) {
		//攻撃
		if (ins.IsTrigerrDown("attack")) {
			//攻撃の生成および状態の設定
			_atk.Attack(character_->GetSpeciesName(), "SwingSword");
			character_->SetState(PlayerChara::STATE::ATTACK);
			//対応するアニメーション
			character_->PlayAnim("atkFirst");
			//時間の設定
			RedyStateCount(static_cast<int>(_atk.GetTotalTime(character_->GetSpeciesName())));
		}

		//回避入力があったとき(ロックオン状態でしか作動しない)
		if (IsDudgeMove() && ins.IsTrigerrDown("jump") && character_->IsLock()) {
			//回避処理
			DoDudge();
		}
		//ロックオン
		if (ins.IsPressed("rock") && lockOn_->CanLockOn()) {
			lockOn_->LockOn();
		}

		if (ins.IsTrigerrUp("rock")) {
			lockOn_->LockOff();
		}
	}
#pragma endregion

#pragma region 能力
	if (ins.IsPressed("ability")) {
		abilityBtnCnt_++;
		//ボタンが一定時間押されていたら
		if (abilityBtnCnt_ > AbilityManager::PRESSED_TIME_4_CHANGE_ABILITY) {
			//能力切り換えフェーズに
			SceneManager::GetInstance().PushScene(std::make_shared<SelectAbility>(*ability_));
			abilityBtnCnt_ = 0;
		}
	}
	else if (ins.IsTrigerrUp("ability")) {
		AbilityManager::STATE nextState = AbilityManager::STATE::END;
		//能力がまだ使用されていないとき
		if (abilityState == AbilityManager::STATE::END) {
			//能力使用準備
			nextState = AbilityManager::STATE::REDY;
		}

		//状態遷移
		ability_->ChangeState(nextState);
	}

	//能力の使用
	if (ins.IsTrigerrDown("action") && abilityState == AbilityManager::STATE::REDY) {
		ability_->ChangeState(AbilityManager::STATE::DIRECTION);
	}
#pragma endregion
}


void PlayerManager::RedyStateCount(const int _limit)
{
	//カウンターの初期化
	stateCnt_ = 0;
	//上限時間の設定
	stateLimit_ = _limit;
}

const bool PlayerManager::IsDudgeMove(void) const
{
	InputManager& ins = InputManager::GetInstance();
	return ins.IsPressed("right") || ins.IsPressed("left") || ins.IsPressed("down")|| ins.IsPressed("up");
}

void PlayerManager::DoDudge(void)
{
	//回避状態に
	character_->SetState(PlayerChara::STATE::DODGE);
	//回避音出す
	SoundManager::GetInstance().Play("Dodge");

	//カメラとキャラクターの前方同士の内積
	auto cFor = SceneManager::GetInstance().GetCamera().GetRot().GetForward();
	auto pFor = character_->GetForward();
	bool isReverse = false;

	float CtoP = Utility::DotF(cFor, pFor);
	if (CtoP < 0.0f) {
		//キャラクターの向きが反転している。
		isReverse = true;
	}

	//プレイヤーからの入力総まとめ
	InputManager& ins = InputManager::GetInstance();


	if (!isReverse) {
		//反転していない場合
		if (ins.IsPressed("left")) {
			//対応するアニメーション
			character_->PlayAnim("dodL");
		}
		else if (ins.IsPressed("right")) {
			//対応するアニメーション
			character_->PlayAnim("dodR");
		}
		else if (ins.IsPressed("down")) {
			//対応するアニメーション
			character_->PlayAnim("dodB");
		}
	}
	else {
		if (ins.IsPressed("left")) {
			//対応するアニメーション
			character_->PlayAnim("dodR");
		}
		else if (ins.IsPressed("right")) {
			//対応するアニメーション
			character_->PlayAnim("dodL");
		}
		else if (ins.IsPressed("up")) {
			//対応するアニメーション
			character_->PlayAnim("dodB");
		}
	}

	//時間の設定
	RedyStateCount(LIMIT_AVOID_STATE);
}

const bool PlayerManager::IsAlive(void) const
{
	return character_->IsAlive();
}

const bool PlayerManager::IsUseAbility(void) const
{
	using State = AbilityManager::STATE;
	auto abilityState = ability_->GetAbilityState();
	return abilityState == State::REDY || abilityState == State::DIRECTION || abilityState == State::USE;
}

const bool PlayerManager::IsUseMagnet(void) const
{
	return ability_->IsUseMagnet();
}




void PlayerManager::DrawDebug(void)
{
	character_->DrawDebug();
}
