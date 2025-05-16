#include"../Manager/GameSystem/PlayerManager.h"
#include"../Manager/GameSystem/EnemyManager.h"
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
	player_ = std::make_unique<PlayerManager>(*this);
	player_->Init();

	enemy_ = std::make_unique<EnemyManager>();
	enemy_->Init(player_->GetPos());
	nearEnemyNum_ = -1;

	//カメラの初期設定
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);					//モード選択
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象
	camera.SetFocusPos(player_->GetFocusPoint());				//注視点
}

void Game::Update(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();

	player_->Update();
	enemy_->Update();

	//ロックオン
	if (player_->IsRockOnTrg()) {
		//各種状態変化と対象の検索
		player_->RockOn();
		camera.ChangeMode(Camera::MODE::ROCKON);
		DecideRockEnemy();
	}
	else if (player_->IsRockOffTrg()) {
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
	else /*if (mode == Camera::MODE::ROCKON)*/ {
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
	SceneManager::GetInstance().GetCamera().DrawDebug();
	player_->DrawDebug();
	enemy_->DrawDebug();
}
