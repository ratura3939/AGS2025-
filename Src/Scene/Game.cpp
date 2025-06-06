#include"../Manager/GameSystem/PlayerManager.h"
#include"../Manager/GameSystem/EnemyManager.h"
#include"../Manager/GameSystem/AttackManager.h"
#include"../Manager/GameSystem/CollisionManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"
#include"../Object/Stage/Stage.h"
#include "Game.h"

Game::Game(void)
{
	nearEnemyNum_ = -1;
	isSlowEffect_ = false;
	slowCnt_ = -1;
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
}

void Game::Update(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();

#pragma region シーン遷移
	//プレイヤーが死んでいたら
	if (!player_->IsAlive()) {
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}
	//敵がいなくなったら
	if (enemy_->GetEnemys().size() <= 0) {
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}
#pragma endregion


#pragma region 基礎アプデ
	player_->Update(*atkMng_);
	//敵はスローの効果を受ける
	if (isSlowEffect_) {
		//スロー時の更新
		slowCnt_++;
		if (slowCnt_ >= LIMIT_SLOW)isSlowEffect_ = false;
		//ある程度の感覚だけ更新する
		if(slowCnt_%UPDATE_INTERVAL_SLOW<= UPDATE_INTERVAL_SLOW)enemy_->Update(player_->GetPos(), *atkMng_);
	}
	else {
		//通常更新
		enemy_->Update(player_->GetPos(), *atkMng_);
	}
	atkMng_->Update();

	//判定処理/その中でスロー演出が入るかどうか
	if (collision_->Collision(player_->GetPlayer(), enemy_->GetEnemys(), atkMng_->GetActiveAttacks())) {
		//スロー演出準備
		slowCnt_ = 0;
		isSlowEffect_ = true;
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
	stage_->Draw();
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
	SceneManager::GetInstance().GetCamera().DrawDebug();
	if (isSlowEffect_) {
		DrawString(0, 140, "NOW_SLOW", 0xffffff);
	}

	//player_->DrawDebug();
	enemy_->DrawDebug();
	atkMng_->DrawDebug();
}
