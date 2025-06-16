#include<EffekseerForDXLib.h>
#include"../Manager/GameSystem/PlayerManager.h"
#include"../Manager/GameSystem/EnemyManager.h"
#include"../Manager/GameSystem/AttackManager.h"
#include"../Manager/GameSystem/CollisionManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Decoration/SoundManager.h"
#include"../Manager/Decoration/EffectManager.h"
#include"../Object/Stage/Stage.h"
#include "Game.h"

Game::Game(void)
{
	nearEnemyNum_ = -1;
	isSlowEffect_ = false;
	slowCnt_ = -1;
	nextBgmVol_ = 0;
}

Game::~Game(void)
{
}

void Game::Init(void)
{
	//生成
	//プレイヤー
	player_ = std::make_unique<PlayerManager>(*this);
	player_->Init();

	//敵
	enemy_ = std::make_unique<EnemyManager>();
	enemy_->Init();
	nearEnemyNum_ = -1;

	//攻撃
	atkMng_ = std::make_unique<AttackManager>();
	//攻撃の登録
	AttackDataInit();

	//判定
	collision_ = std::make_unique<CollisionManager>();

	stage_ = std::make_unique<Stage>();
	stage_->Init();

	//カメラの初期設定
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);					//モード選択
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象
	camera.SetFocusPos(player_->GetFocusPoint());				//注視点

	//音関係初期設定
	InitSound();
	//エフェクト関係初期化
	InitEffect();
}

void Game::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	//BGM
	sndM.Add(SoundManager::TYPE::BGM, "NomalBgm",
		rsM.Load(ResourceManager::SRC::GAME_BGM).handleId_);
	//バトルBGM
	sndM.Add(SoundManager::TYPE::BGM, "BattleBgm",
		rsM.Load(ResourceManager::SRC::BATTLE_BGM).handleId_);

	//初手は普通のBGM
	sndM.Play("NomalBgm");
	nowBgmStr_ = "NomalBgm";
	switchBgmStr_ = "BattleBgm";

	//SE
	//歩く
	sndM.Add(SoundManager::TYPE::SE, "Walk",
		rsM.Load(ResourceManager::SRC::WALK_SE).handleId_,20);
	sndM.AdjustVolume("Walk",60);

	//走る
	sndM.Add(SoundManager::TYPE::SE, "Dush",
		rsM.Load(ResourceManager::SRC::RUN_SE).handleId_,10);
	sndM.AdjustVolume("Dush", 60);

	//剣を振る
	sndM.Add(SoundManager::TYPE::SE, "SwingSword",
		rsM.Load(ResourceManager::SRC::SWING_SWORD_SE).handleId_);
	//ロックオン
	sndM.Add(SoundManager::TYPE::SE, "RockOn",
		rsM.Load(ResourceManager::SRC::ROCK_ON_SE).handleId_);
	//プレイヤーを発見
	sndM.Add(SoundManager::TYPE::SE, "FindPlayer",
		rsM.Load(ResourceManager::SRC::FIND_PLAYER_SE).handleId_);
	//ダメージ
	sndM.Add(SoundManager::TYPE::SE, "Damage",
		rsM.Load(ResourceManager::SRC::DAMAGE_SE).handleId_);

}

void Game::InitEffect(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	EffectManager& efcM = EffectManager::GetInstance();

	//剣
	efcM.Add("Sword", rsM.Load(ResourceManager::SRC::SWORD_EFC).handleId_);
	//ダメージ
	efcM.Add("Damage", rsM.Load(ResourceManager::SRC::DAMAGE_EFC).handleId_);
}

void Game::Update(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	Camera& camera = scM.GetCamera();

#pragma region シーン遷移
	//プレイヤーが死んでいたら
	if (!player_->IsAlive()) {
		//BGM念のため両方停止
		sndM.Stop(nowBgmStr_);
		sndM.Stop(switchBgmStr_);
		//シーン遷移
		scM.ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}
	//敵がいなくなったら
	if (enemy_->GetEnemys().size() <= 0) {
		sndM.Stop(nowBgmStr_);
		sndM.Stop(switchBgmStr_);
		//シーン遷移
		scM.ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}
#pragma endregion


#pragma region 基礎アプデ
	player_->Update(*atkMng_);
	//敵はスローの効果を受ける
	if (isSlowEffect_) {
		//スロー時の更新(このカウンタはスローの影響を受けない)
		slowCnt_++;
		if (slowCnt_ >= LIMIT_SLOW) {
			isSlowEffect_ = false;
			//更新処理を100％にもどす
			scM.SetUpdateSpeedRate_(NOMAL_SPEED_PERCENT);
			enemy_->SetAnimSpeedRate(scM.GetUpdateSpeedRatePercent_());
		}
	}
	//敵
	enemy_->Update(player_->GetPos(), *atkMng_);
	//攻撃
	atkMng_->Update();

	//判定処理/その中でスロー演出が入るかどうか
	if (collision_->Collision(player_->GetPlayer(), enemy_->GetEnemys(), atkMng_->GetActiveAttacks())) {
		//スロー演出準備
		slowCnt_ = 0;
		isSlowEffect_ = true;
		//更新スピードを50％に設定
		scM.SetUpdateSpeedRate_(SLOW_SPEED_PERCENT);
		//敵もそれに対応
		enemy_->SetAnimSpeedRate(scM.GetUpdateSpeedRatePercent_());

	}

	
#pragma endregion

#pragma region BGM
	

	//敵の状態(戦闘・それ以外)のトリガ
	if (enemy_->IsSwitchBattleOrNomalEnemyTrg()) {
		//もともと切り換え中だったら
		if (switchBgm_) {
			//強制終了処理
			FinishSwitchBgm();
		}

		//切り換え開始
		switchBgm_ = true;
		//切り替え後の再生
		sndM.Play(switchBgmStr_);
	}
	//BGM切り換え実行中
	if (switchBgm_) {
		//音量調整に加算
		nextBgmVol_ += BGM_VOL_ACC;
		sndM.AdjustVolume(switchBgmStr_, nextBgmVol_);			//次のBGMは音量をあげる
		sndM.AdjustVolume(nowBgmStr_, (100 - nextBgmVol_));	//現在のBGMは音量を下げる

		//もしボリュームが100以上なら
		if (nextBgmVol_ >= 100) {
			nextBgmVol_ = 100;	//音量を100％に
			//終了処理
			FinishSwitchBgm();
		}
	}
#pragma endregion


	//TODO
	// カメラのロックオンの処理の最適化
	//ロックオン関係
	//押下時
	if (InputManager::GetInstance().IsTrigerrDown("rock")) {
		//敵がいるとき
		if (enemy_->GetEnemys().size() > 0) {
			//対象の検索
			nearEnemyNum_ = DecideRockEnemy();
			//近くに敵がいるとき
			if (nearEnemyNum_ >= 0) {
				SoundManager::GetInstance().Play("RockOn");
				RockOn();
			}
		}
	}
	//押下終了時
	else if (InputManager::GetInstance().IsTrigerrUp("rock")) {
		//各種状態の変化
		RockOff();
	}

	//カメラがロックオン状態のとき敵がいなかったら
	if (camera.GetMode() == Camera::MODE::ROCKON && DecideRockEnemy() < 0) {
		//各種状態の変化
		RockOff();
	}

	//カメラの設定
	camera.SetFollow(player_->GetPos(),player_->GetQua());		//追従対象の更新

	Camera::MODE mode = camera.GetMode();
	//追従時
	if (mode == Camera::MODE::FOLLOW) {
		camera.SetFocusPos(player_->GetFocusPoint());//注視点の更新
	}
	//ロックオン時
	else if (mode == Camera::MODE::ROCKON){
		camera.SetRockPos(enemy_->GetPos(nearEnemyNum_));	//ロックオン対象の設定
	}
	
}

void Game::Draw(void)
{
	//stage_->Draw();
	enemy_->Draw();
	player_->Draw();

	DrawDebug();
}

void Game::Release(void)
{
	player_->Release();
	enemy_->Release();
}

void Game::AttackDataInit(void)
{
	atkMng_->AddAttack(PlayerManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD, false, PlayerManager::ATTACK_TIME);
	atkMng_->AddAttack(EnemyManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD, false, EnemyManager::ATTACK_TIME, EnemyManager::ATTACK_TIME_START, EnemyManager::ATTACK_TIME_END);
}

const int Game::DecideRockEnemy(void)
{
	return enemy_->GetNearEnemyNum(player_->GetPos());
}

void Game::FinishSwitchBgm(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	//切り換え終了
	switchBgm_ = false;
	sndM.AdjustVolume(switchBgmStr_, nextBgmVol_);
	sndM.Stop(nowBgmStr_);	//今まで流していたものを停止
	//現在のBGM名と切り替え後のBGM名の切り換え
	//後々ボス個体の物も用意するのでそこで要調整
	auto ret = nowBgmStr_;
	nowBgmStr_ = switchBgmStr_;
	switchBgmStr_ = ret;
	//初期化
	nextBgmVol_ = 0;
}

void Game::RockOn(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();
	player_->RockOn();
	camera.ChangeMode(Camera::MODE::ROCKON);
}

void Game::RockOff(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();
	player_->RockOff();
	camera.ChangeMode(Camera::MODE::FOLLOW);
}


void Game::DrawDebug(void)
{
	//SceneManager::GetInstance().GetCamera().DrawDebug();
	//if (isSlowEffect_) {
	//	DrawString(0, 140, "NOW_SLOW", 0xffffff);
	//}

	////player_->DrawDebug();
	//enemy_->DrawDebug();
	//atkMng_->DrawDebug();

}
