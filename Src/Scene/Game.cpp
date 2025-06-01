#include"../Manager/GameSystem/PlayerManager.h"
#include"../Manager/GameSystem/EnemyManager.h"
#include"../Manager/GameSystem/AttackManager.h"
#include"../Manager/GameSystem/CollisionManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Generic/SceneManager.h"
#include "Game.h"

Game::Game(void)
{
	nearEnemyNum_ = -1;
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


	//カメラの初期設定
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);					//モード選択
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象
	camera.SetFocusPos(player_->GetFocusPoint());				//注視点
}

void Game::Update(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();

	//プレイヤーが死んでいたら
	if (!player_->IsAlive()) {
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	player_->Update(*atkMng_);
	enemy_->Update(player_->GetPos(), *atkMng_);
	atkMng_->Update();

	//判定
	collision_->Collision(player_->GetPlayer(), enemy_->GetEnemys(), atkMng_->GetActiveAttacks());

	//TODO
	// カメラのロックオンの処理の最適化
	//ロックオン関係
	if (player_->IsRockOnTrg()) {
		if (camera.GetMode() != Camera::MODE::ROCKON) {
			//敵がいるとき
			if (enemy_->GetEnemys().size() > 0) {
				//各種状態変化と対象の検索
				player_->RockOn();
				camera.ChangeMode(Camera::MODE::ROCKON);
				DecideRockEnemy();
			}
		}
	}
	else {
		//各種状態の変化
		player_->RockOff();
		camera.ChangeMode(Camera::MODE::FOLLOW);
	}

	//カメラがロックオン状態のとき敵がいなかったら
	if (camera.GetMode() == Camera::MODE::ROCKON && enemy_->GetEnemys().size() <= 0) {
		//各種状態の変化
		player_->RockOff();
		camera.ChangeMode(Camera::MODE::FOLLOW);
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
	atkMng_->AddAttack(EnemyManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD, false, EnemyManager::ATTACK_TIME);
}

void Game::DecideRockEnemy(void)
{
	nearEnemyNum_ = enemy_->GetNearEnemyNum(player_->GetPos());
	//近くにてきがいないときは
	if (nearEnemyNum_ == -1) {
		SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
		player_->RockOff();
	}
}


void Game::DrawDebug(void)
{
	//SceneManager::GetInstance().GetCamera().DrawDebug();
	//player_->DrawDebug();
	enemy_->DrawDebug();
	atkMng_->DrawDebug();
}
