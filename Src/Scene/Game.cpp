#include"../Manager/GameSystem/PlayerManager.h"
#include"../Manager/GameSystem/EnemyManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Generic/SceneManager.h"
#include "Game.h"

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::Init(void)
{
	//生成
	player_ = std::make_unique<PlayerManager>();
	player_->Init();

	enemy_ = std::make_unique<EnemyManager>();
	enemy_->Init();
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

	//ロックオンの切り替え
	if (player_->IsRockOnTrg()) {
		player_->RockOn();
		camera.ChangeMode(Camera::MODE::ROCKON);
		DecideRockEnemy();
	}
	else if (player_->IsRockOffTrg()) {
		player_->RockOff();
		camera.ChangeMode(Camera::MODE::FOLLOW);
	}

	//カメラの設定
	camera.SetFollow(player_->GetPos(),player_->GetQua());		//追従対象の更新

	//ロックオン時は注視点は処理で勝手に更新する
	//そのため敵の位置情報だけを受け渡す
	Camera::MODE mode = camera.GetMode();
	if (mode == Camera::MODE::FOLLOW)camera.SetFocusPos(player_->GetFocusPoint());//注視点の更新
	else if (mode == Camera::MODE::ROCKON)enemy_->GetPos(nearEnemyNum_);
	
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
