#include"../Manager/GameSystem/PlayerManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Generic/SceneManager.h"
#include "Game.h"

Game::Game(void)
{
	//確認用これはAGS2025夏用のプロジェクトです
	//確認用Part２
}

Game::~Game(void)
{
}

void Game::Init(void)
{
	//プレイヤーの生成
	player_ = std::make_unique<PlayerManager>();
	player_->Init();

	//カメラの初期設定
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
	//カメラにプレイヤーの設定する
	SceneManager::GetInstance().GetCamera().SetFollow(player_->GetPos(), player_->GetQua());
}

void Game::Update(void)
{
	//プレイヤー
	player_->Update();
	//カメラにプレイヤーの設定する
	SceneManager::GetInstance().GetCamera().SetFollow(player_->GetPos(),player_->GetQua());
}

void Game::Draw(void)
{
	DrawString(0, 0, "GameScene", 0xffffff, true);
	//プレイヤー
	player_->Draw();
}

void Game::Release(void)
{
	//プレイヤー
	player_->Release();
}
