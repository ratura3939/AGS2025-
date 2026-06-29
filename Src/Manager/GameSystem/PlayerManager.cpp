#include"../../Object/Character/Player/PlayerChara.h"
#include"../Generic/InputManager.h"
#include"../Generic/Camera.h"
#include"../Generic/ResourceManager.h"
#include"../Decoration/UIManager2d.h"
#include"../Decoration/SoundManager.h"
#include"../GameSystem/AttackManager.h"
#include"../../PlayerSystem/LockOnManager.h"
#include"../../PlayerSystem/AbilityManager.h"
#include"../../Scene/Main/Game.h"
#include"../../Scene/Sub/SelectAbility.h"
#include"../../Utility/Utility.h"
#include "PlayerManager.h"

const std::string PlayerManager::ATTACK_NOMAL = "PlayerAttack";

//ローカル定数
namespace {
	const std::string UI_OPERATION = "Operation";
	const VECTOR UI_OPERATION_POS = { 150.0f,500.0f,0.0f };
	const float UI_OPERATION_EX = 1.3f;
}

PlayerManager::PlayerManager(Game& _gameScene, EnemyManager& _enemy, AttackManager& _atk, StageManager& _stage)
	:scene_(_gameScene)
	,atkMng_(_atk)
	,character_(std::make_shared<PlayerChara>(_atk))
	,lockOn_(std::make_unique<LockOnManager>(_gameScene, *this, _enemy))
	,ability_(std::make_unique<AbilityManager>(_stage, *character_))
	,stateCnt_(0)
	,stateLimit_(0)
	,abilityBtnCnt_(0)
{
	character_->Init();
	operationImgs_[static_cast<int>(SceneManager::CNTL::KEY)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::KEY_OPERATION_IMG).handleId_;
	operationImgs_[static_cast<int>(SceneManager::CNTL::PAD)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::PAD_OPERATION_IMG).handleId_;
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
	//攻撃とコライダーの紐づけ
	atkMng_.AddAttackCollider(character_->GetSpeciesName(), character_->GetAttackCollider(), false, ATTACK_TIME);

	//操作UIの設定
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.Add(UI_OPERATION, operationImgs_[static_cast<int>(SceneManager::CNTL::KEY)], UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_OPERATION, UI_OPERATION_POS, UI_OPERATION_EX);
	//操作UIの画像設定
	SetOperationUI(SceneManager::GetInstance().GetController());
}

void PlayerManager::Update(void)
{
	//状態管理
	//通常じゃないとき
	if (character_->GetState() != PlayerChara::STATE::NORMAL) {
		//カウンタが状態の上限時間を上回っていたら
		if (stateCnt_ > stateLimit_) {
			//通常に戻す
			character_->SetState(PlayerChara::STATE::NORMAL);
		}
		stateCnt_++;
	}

	//スロー状態管理
	if (character_->IsSlow()) {
		scene_.StartSlow();		//回避状態開始
		character_->SetIsSlow(false);
	}

	//プレイヤーからの入力
	UserInput();
	//カメラ前方に強制的に向かせるか
	bool isForceFacingCamera = !ability_->IsPlayerAnyInput() || character_->GetLockState() == PlayerChara::LOCK_STATE::LOCKON;
	character_->SetIsForceFacingCamera(isForceFacingCamera);
	//キャラクター更新
	character_->Update();
	//ロックオン更新
	lockOn_->Update();
	//能力更新
	ability_->Update();
}

void PlayerManager::Draw(void)
{
	character_->Draw();	//キャラクター
	ability_->Draw();	//アビリティ

	//操作UIの描画
	UIManager2d::GetInstance().Draw(UI_OPERATION);
}

void PlayerManager::Release(void)
{
	character_->Release();
}

const VECTOR& PlayerManager::GetPos(void)const
{
	return character_->GetPos();
}

const Quaternion& PlayerManager::GetQua(void)
{
	return character_->GetQua();
}

std::weak_ptr<PlayerChara> PlayerManager::GetPlayer(void)
{
	return character_;
}

const VECTOR& PlayerManager::GetFocusPoint(void)
{
	return character_->GetFocusPoint();
}

const VECTOR& PlayerManager::GetFollowPosForUseMagnet(void)
{
	return ability_->GetFollowPosForUseMagnet(character_->GetPos());
}


void PlayerManager::RedyLockOn(void)
{
	character_->ChangeLockState(true);
}

void PlayerManager::RedyLockOff(void)
{
	character_->ChangeLockState(false);
}

void PlayerManager::SetOperationUI(const SceneManager::CNTL& _operation)
{
	UIManager2d::GetInstance().SetImage(UI_OPERATION, operationImgs_[static_cast<int>(_operation)]);
}

void PlayerManager::UserInput(void)
{
	//プレイヤーからの入力総まとめ
	InputManager& ins = InputManager::GetInstance();

#pragma region 移動
	//移動
	//入力
	auto moveVec = ins.GetMoveInput();
	//移動がある時
	if (moveVec.x != 0.0f || moveVec.y != 0.0f) {
		//キャラクターに移動方向を渡す
		character_->InputMoveVec(VECTOR(moveVec.x, moveVec.y, 0.0f));
	}
	//移動していないとき
	else {
		//移動なしの設定
		character_->InputMoveDir(PlayerChara::MOVE_DIR::MAX);
	}
	
	//ダッシュ
	character_->InputDash(ins.IsPressed("dash"));

	//ジャンプ入力(ロックオンではないとき)
	if(character_->GetLockState() != PlayerChara::LOCK_STATE::LOCKON&& ins.IsTrigerrDown("jump"))
	{
		//ジャンプ処理
		character_->Jump();
	}
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
			atkMng_.Attack(character_->GetSpeciesName(), "SwingSword");
			character_->SetState(PlayerChara::STATE::ATTACK);
			//対応するアニメーション
			character_->PlayAnim("atkFirst");
			//時間の設定
			RedyStateCount(static_cast<int>(atkMng_.GetTotalTime(character_->GetSpeciesName())));
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
		//ロックオンの解除
		if (ins.IsTrigerrUp("rock")) {
			lockOn_->LockOff();
		}
	}
#pragma endregion

#pragma region 能力
	//アビリティ使用ボタン押下している間
	if (ins.IsPressed("ability")) {
		abilityBtnCnt_++;
		//ボタンが一定時間押されていたら
		if (abilityBtnCnt_ > AbilityManager::PRESSED_TIME_4_CHANGE_ABILITY) {
			//能力切り換えフェーズに
			SceneManager::GetInstance().PushScene(std::make_shared<SelectAbility>(*ability_));
			abilityBtnCnt_ = 0;
		}
	}
	//アビリティ使用ボタン離し時
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
		//使用演出へ
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

	auto moveVec = character_->GetInputMoveDir();
	using CHARA_DIR = PlayerChara::MOVE_DIR;

	//各方向に応じた回避アニメーションを再生
	if(moveVec == CHARA_DIR::LEFT) {
		character_->PlayAnim("dodL");
	}
	else if (moveVec == CHARA_DIR::RIGHT) {
		character_->PlayAnim("dodR");
	}
	else if (moveVec == CHARA_DIR::BACK) {
		character_->PlayAnim("dodB");
	}
	else {
		//念のために前に回避入力があったときのアニメーションを出す
		character_->PlayAnim("dodB");
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